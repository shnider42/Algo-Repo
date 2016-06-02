param n;
param costBound;
param cost {i in 0..n-1};
param value {i in 0..n-1};
var x {i in 0..n-1} binary;
maximize objective: sum {i in 0..n-1} x[i]*value[i];
subject to c1: sum {i in 0..n-1} x[i]*cost[i] <= costBound;