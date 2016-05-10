module top( a , b , c , o);

input a , b , c;

output o;

wire a1 , b1 , c1 , d , e , f , g , h;


and(d,a,b1,c1);
and(e,a1,b1,c);
and(f,a,b,c);
and(g,a1,b,c1);
or(h,d,e,f,g);
not(a1,a);
not(b1,b);
not(c1,c);
not(o,h);

endmodule 
