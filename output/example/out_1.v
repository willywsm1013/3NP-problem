module top(a,b,c,o);
input a,b,c;
output o;
wire d,e,f,g;
wire ff,gg;
and (d,a,b);
xor (e,a,b);
xor (f,d,e);
nand (g,b,cc);
xor (o,ff,gg);
_cut cut_1_1 (ff,f)
_cut cut_0_1 (gg,g)
endmodule
