// Authored illustration only: not parsed, simulated, synthesized or proved here.
module saturating_counter #(parameter int WIDTH = 3) (
    input logic clk, reset, enable,
    output logic [WIDTH-1:0] value
);
    always_ff @(posedge clk) begin
        if (reset) value <= '0;
        else if (enable && !(&value)) value <= value + 1'b1;
    end
endmodule
