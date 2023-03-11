/******************************************************************************
Copyright (c) 2019 Georgia Instititue of Technology
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Author : Hyoukjun Kwon (hyoukjun@gatech.edu)
*******************************************************************************/


#ifndef MAESTRO_OPTION_HPP_
#define MAESTRO_OPTION_HPP_

#include <iostream>
#include <list>
#include <string>

#include <boost/program_options.hpp>

namespace maestro {


  namespace po = boost::program_options;
  class Options {
    public:
      /* Default values : Models MAERI with VGG16 and 64 multiplier switches*/
      int np = 7;                           // number of PEs
      int bw = INT_MAX;                     
      int hops = 1;                         //the average number of NoC hops
      int hop_latency = 0;
      bool mc = true;                       //the multicasting capability of NoC
      bool top_bw_only = false;             //Only constraint top bandwidth             ？？？
      bool bw_sweep = false;                //Sweep the NoC bandwidth                   ？？？

      bool full_buffer = false;             //Use a large buffer to host all the data points
      std::list<std::string> in_tensors = {"weight", "input"};
      std::list<std::string> out_tensors = {"output"};

      std::string dataflow_file_name = "data/dataflow/maeri.m";
      std::string layer_file_name = "data/layer/vgg16_conv1.m";
      std::string dfsl_file_name = "";
      std::string hw_file_name = "";

                                            //   ？？？？？？最后一个
      int num_simd_lanes = 1;               //the number of ALUs in each PE 
      bool do_reduction = true;             //"If the problem requires reduction or not"
      bool do_implicit_reduction = true;    //If PEs reduce items as soon as they generate partial results; if set as true, reductions do not require additional cycles.立即做归约运算
      bool fg_sync = false;                 //"Fine-grained synchronization is performed (future work)" 

      bool do_dse = true;
      bool do_print_ds = false;                    //Print out entire design space into a file
      int l1_size = INT_MAX;
      int l2_size = INT_MAX;
      int min_num_pes = 1024;
      int min_noc_bw = 512;
      int max_num_pes = 1024;
      int max_noc_bw = 512;
      double area_cap = 1000000.0; // unit: um^2
      double power_cap = 10000.0; // unit: mW
      std::string optimization_target = "runtime";//Optimization target (available options: runtime, energy, performance/energy)
      bool verbose = false;                       //Printout intermediate results
      bool print_design_space_to_file = false;    //Print out the valid design space into an output file
      bool print_res_to_screen = true;
      bool print_res_to_csv_file = true;
      bool print_log_file = false;
      int message_print_lv = 0;
      int pe_tick = 4;                            //The granularity of num PE search
      int bw_tick = 4;                            //The granularity of bw search
      //felix 
      int offchip_bw = 70000;


      bool parse(int argc, char** argv)
      {
          std::vector<std::string> config_fnames;
          
          //构造  选项描述器 ，参数为该描述器的名字，比如此描述器为General Options
          po::options_description desc("General Options");
          desc.add_options()    //为选项描述器增加选项，其参数依次为: key, value的类型，该选项的描述，参数输入时，如：--help
              ("help", "Display help message")
              //value<type>(ptr) : ptr为该选项对应的外部变量的地址, 当该选项被解析后, 
                                    //可通过下面的notify()函数将选项的值赋给该外部变量,该变量的值会自动更新
              ("print_res", po::value<bool>(&print_res_to_screen) ,"Print the eval results to screen")
              ("print_res_csv_file", po::value<bool>(&print_res_to_csv_file) ,"Print the eval results to screen")
              ("print_log_file", po::value<bool>(&print_log_file) ,"Print detailed logs to a file")
              ("msg_print_lv", po::value<int>(&message_print_lv) ,"the name of dataflow description file")
          ;

          po::options_description io("File IO options");
          io.add_options()
            ("dataflow_file", po::value<std::string>(&dataflow_file_name) ,"the name of dataflow description file")
            ("layer_file", po::value<std::string>(&layer_file_name) ,"the name of layer dimension description file")
            ("Mapping_file", po::value<std::string>(&dfsl_file_name), "the name of DFSL file")
            ("HW_file", po::value<std::string>(&hw_file_name), "the name of hardware description file (temporary feature)")
          ;

          po::options_description nocs("Network on chip options");
          nocs.add_options()
            //felix
            ("offchip_bw_cstr", po::value<int>(&offchip_bw), "the bandwidth of Off-Chip")
            ("noc_bw_cstr", po::value<int>(&bw), "the bandwidth of NoC")
            ("noc_hops", po::value<int>(&hops), "the average number of NoC hops")
            ("noc_hop_latency", po::value<int>(&hop_latency), "the latency for each of NoC hop")
            ("noc_mc_support", po::value<bool>(&mc), "the multicasting capability of NoC")
            ("top_bw_only", po::value<bool>(&top_bw_only), "Only constraint top bandwidth")
            ("bw_sweep", po::value<bool>(&bw_sweep), "Sweep the NoC bandwidth")
          ;

          po::options_description pe_array("Processing element options");
          pe_array.add_options()
            ("num_pes", po::value<int>(&np), "the number of PEs")
            ("num_simd_lanes", po::value<int>(&num_simd_lanes), "the number of ALUs in each PE")
            ("do_implicit_reduction", po::value<bool>(&do_implicit_reduction), "If PEs reduce items as soon as they generate partial results; if set as true, reductions do not require additional cycles.")
            ("do_fg_sync", po::value<bool>(&fg_sync), "Fine-grained synchronization is performed (future work)")
          ;

          po::options_description problem("Problem description options");
          problem.add_options()
            ("do_reduction_op", po::value<bool>(&do_reduction), "If the problem requires reduction or not")
          ;

          po::options_description dse("Design Space Exploration options");
          dse.add_options()
            ("full_buffer", po::value<bool>(&full_buffer), "Use a large buffer to host all the data points")
            ("do_dse", po::value<bool>(&do_dse), "Perform DSE")
            ("verbose", po::value<bool>(&verbose), "Printout intermediate results")
            ("print_design_space", po::value<bool>(&print_design_space_to_file), "Print out the valid design space into an output file")
            ("print_space", po::value<bool>(&do_print_ds), "Print out entire design space into a file")
            ("l1_size_cstr", po::value<int>(&l1_size), "L1 memory size in Bytes")
            ("l2_size_cstr", po::value<int>(&l2_size), "L2 memory size in Bytes")
            ("min_num_PEs", po::value<int>(&min_num_pes), "The max number of PEs during DSE")
            ("min_noc_bw", po::value<int>(&min_noc_bw), "The maximum noc bandwidth during DSE")
            ("max_num_PEs", po::value<int>(&max_num_pes), "The max number of PEs during DSE")
            ("max_noc_bw", po::value<int>(&max_noc_bw), "The maximum noc bandwidth during DSE")
            ("pe_tick", po::value<int>(&pe_tick), "The granularity of num PE search")
            ("bw_tick", po::value<int>(&bw_tick), "The granularity of bw search")
            ("area_constraint", po::value<double>(&area_cap), "Area budget")
            ("power_constraint", po::value<double>(&power_cap), "Power budget")
            ("optimization_target", po::value<std::string>(&optimization_target), "Optimization target (available options: runtime, energy, performance/energy)")
          ;

          //============================总的选项描述器============================
          po::options_description all_options;
          all_options.add(desc);
          all_options.add(io);
          all_options.add(nocs);
          all_options.add(pe_array);
          all_options.add(problem);
          all_options.add(dse);

          //选项存储器，,继承自map容器
          po::variables_map vm;

          //对命令行输入的参数做解析,而后将其存入选项存储器
          //parse_command_line()对输入的选项做解析
          //store()将解析后的结果存入选项存储器
          po::store(po::parse_command_line(argc, argv, all_options), vm);

          //参数解析完成后，通知variables_map去更新所有的外部变量
          po::notify(vm);

          //此处总是返回true        // /如果输入了未定义的选项，程序会抛出异常，所以对解析代码parse_command_line要用try-catch块包围，如果异常就返回0
          return true;
      }
  }; //End of class Options
}; //End of namespace maestro
#endif
