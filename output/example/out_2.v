module top(a,b,c,o);
input a,b,c;
output o;
wire d,e,f,g;
wire ff,gg;
and (d,aa,bb);
xor (e,aa,bb);
or (f,d,e);
and (g,bb,cc);
xnor (o,ff,gg);
_cut cut_1_1 (ff,f)
_cut cut_0_bar_1 (gg,g)
endmodule
