module top ( i1 , i2 , i3 , o );

input i1 , i2 , i3 ;

output o ;

wire t ;

nand ( t, i1 , i2 , i3) ;


buf ( o , t );

endmodule
