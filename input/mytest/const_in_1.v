module top ( a , b , c , o);

input a , b , c;

output o;

wire d;

and(d,a,1'b0);

xnor(o,d,b,c);

endmodule
