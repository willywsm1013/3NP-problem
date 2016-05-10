module top(i1,i2,o);
input i1,i2;
output o;
wire t;
wire tt;
nor (t,i1i1,i2i2);
buf (o,tt);
_cut cut_0_1 (tt,t)
endmodule
