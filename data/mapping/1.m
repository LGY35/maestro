Network Resnet50 {
    Layer CONV2_3_3 {
            Type: CONV
            Stride { X: 1, Y: 1 }		
            Dimensions { K: 256, C: 64, R: 1, S: 1, Y: 56, X: 56 }
            Dataflow {
                SpatialMap(1,1) K;
                TemporalMap(64,64) C;
                SpatialMap(Sz(R),Sz(R)) R;
                TemporalMap(Sz(S),Sz(S)) S;
                TemporalMap(Sz(R),1) Y;
                TemporalMap(Sz(S),1) X;	
                Cluster(64, P);
                SpatialMap(1,1) C;
                TemporalMap(Sz(R),1) Y;
                TemporalMap(Sz(S),1) X;
                SpatialMap(Sz(R),Sz(R)) R;
                TemporalMap(Sz(S),Sz(S)) S;
            }
        }
}

// Network ResNeXt50 {
// 	Layer CONV1 {
// 		Type: CONV
// 		Stride { X: 1, Y: 1 }
// 		Dimensions { K: 1, C: 1, R: 3, S: 1, Y: 4, X: 1 }
// 		Dataflow {
// 			SpatialMap(1,1) X;
// 			TemporalMap(1,1) R;
// 		}
// 	}
// }

