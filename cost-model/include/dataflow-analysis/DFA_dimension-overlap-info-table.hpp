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


#ifndef MAESTRO_DFA_DIMENSION_OVERLAP_INFO_HPP_
#define MAESTRO_DFA_DIMENSION_OVERLAP_INFO_HPP_

#include <memory>
#include <list>
#include <utility>
#include <string>


namespace maestro {
  namespace DFA {

    class DimensionOverlapInfoTable {
      public:
        DimensionOverlapInfoTable() {
          //给一个空的值
          overlapping_dimensions_ = std::make_unique<std::list<std::shared_ptr<std::pair<std::string, std::string>>>>();
        }

        //只添加一对pair（ref-dim 和 sliding-dim）
        void AddOverlapDimension(std::string reference_dim, std::string sliding_dim) {
          auto new_overlap_info = std::make_shared<std::pair<std::string, std::string>>(reference_dim, sliding_dim);
          overlapping_dimensions_->push_back(new_overlap_info);
        }
        //添加一个list的pair
        void AddOverlapDimensions(std::shared_ptr<std::list<std::shared_ptr<std::pair<std::string, std::string>>>> overlap_dim_list) {
          for(auto& it: *overlap_dim_list) {
            overlapping_dimensions_->push_back(it);
          }
        }

        //判断是否存在重叠
        bool IsOverlapped(std::string dim) {
          bool ret = false;

          for(auto& it: *overlapping_dimensions_) {  //it等于list中的一个pair
            auto ref_dim = it->first;                //ref-dim = it->first，也就是pair中的第一个
            auto sliding_dim = it->second;           //silding-dim = it->second，也就是pair中的第一个
            if(dim == ref_dim || dim == sliding_dim) {//如果sliding = ref dim，就是重叠了
              ret = true;
              break;
            }
          }

          return ret;
        }

        //输入一个dim，与overlapping_dimensions_中的sliding_dim比较，判断输入dim是不是sliding_dim中的一个
        bool IsSlidingDim(std::string dim) {
          bool ret = false;

          for(auto& it: *overlapping_dimensions_) {
            auto sliding_dim = it->second;
            if(dim == sliding_dim) {
              ret = true;
            }
          }

          return ret;
        }

        //输入一个dim，与overlapping_dimensions_中的俩个dim比较，返回其名称
        std::string GetCounterPart(std::string dim) {
          std::string ret = "";

          for(auto& it: *overlapping_dimensions_) {
            auto reference_dim = it->first;
            auto sliding_dim = it->second;
            if(dim == sliding_dim) {
              ret = reference_dim;
            }
            else if(dim == reference_dim) {
              ret = sliding_dim;
            }
          }

          return ret;
        }

      protected:
        std::unique_ptr<std::list<std::shared_ptr<std::pair<std::string, std::string>>>> overlapping_dimensions_;

    }; // End of class DiemensionOverlapInfoTable


  }; // End of namespace DFA
};  // End of namespace maestro

#endif
