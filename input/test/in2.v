module top ( i1 , i2 , o );
input i1 , i2 ;
output o ;
wire t ;
or ( t, i1 , i2 ) ;
buf ( o , t );
endmodule
