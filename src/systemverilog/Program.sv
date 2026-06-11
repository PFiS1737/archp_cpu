import "DPI-C" function int unsigned get_instruction(input int unsigned pc);

module Program (
    input var  int unsigned pc,
    output var int unsigned instr
);

  always_comb instr = get_instruction(pc);

endmodule
