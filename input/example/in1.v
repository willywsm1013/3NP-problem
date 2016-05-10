module top ( a , b , c , o);

input a , b , c;

output o;

wire d , e , f , g;

and  (d,a,b);

xor  (e,a,b);

xor  (f,d,e);

nand (g,b,c);

xor  (o,f,g);

endmodule
