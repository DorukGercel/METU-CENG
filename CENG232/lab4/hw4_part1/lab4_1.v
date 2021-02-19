`timescale 1ns / 1ps

module MaskROM (input [3:0] RomAddress, output reg[7:0] RomDataOut);
/*  Please write your code below  */

always@(RomAddress)begin
if(RomAddress == 4'b0000)begin
	RomDataOut = 8'b00000000;
end
else if(RomAddress == 4'b0001)begin
	RomDataOut = 8'b00001111;
end
else if(RomAddress == 4'b0010)begin
	RomDataOut = 8'b00011110;
end
else if(RomAddress == 4'b0011)begin
	RomDataOut = 8'b00110000;
end
else if(RomAddress == 4'b0100)begin
	RomDataOut = 8'b01010000;
end
else if(RomAddress == 4'b0101)begin
	RomDataOut = 8'b01100110;
end
else if(RomAddress == 4'b0110)begin
	RomDataOut = 8'b01101010;
end
else if(RomAddress == 4'b0111)begin
	RomDataOut = 8'b01111110;
end
else if(RomAddress == 4'b1000)begin
	RomDataOut = 8'b10000001;
end
else if(RomAddress == 4'b1001)begin
	RomDataOut = 8'b10100000;
end
else if(RomAddress == 4'b1010)begin
	RomDataOut = 8'b10100110;
end
else if(RomAddress == 4'b1011)begin
	RomDataOut = 8'b10111101;
end
else if(RomAddress == 4'b1100)begin
	RomDataOut = 8'b11000000;
end
else if(RomAddress == 4'b1101)begin
	RomDataOut = 8'b11010000;
end
else if(RomAddress == 4'b1110)begin
	RomDataOut = 8'b11010011;
end
else if(RomAddress == 4'b1111)begin
	RomDataOut = 8'b11100110;
end
end

/*  Please write your code above  */
endmodule


/*mode 0:read, 1:write*/																							
module RgbRAM (input Mode,input [3:0] RamAddress, input [23:0] RamDataIn,
					input [7:0] Mask,input [2:0] Op, input CLK, output reg [23:0] RamDataOut);
/*  Please write your code below  */
reg[23:0]Cmem[0:15];
reg[9:0] extra;
reg[1:0] digit;

initial begin
	Cmem[0][23:0] = 24'b00000000000000000000000;
	Cmem[1][23:0] = 24'b00000000000000000000000;
	Cmem[2][23:0]= 24'b00000000000000000000000;
	Cmem[3][23:0] = 24'b00000000000000000000000;
	Cmem[4][23:0] = 24'b00000000000000000000000;
	Cmem[5][23:0] = 24'b00000000000000000000000;
	Cmem[6][23:0] = 24'b00000000000000000000000;
	Cmem[7][23:0] = 24'b00000000000000000000000;
	Cmem[8][23:0] = 24'b00000000000000000000000;
	Cmem[9][23:0] = 24'b00000000000000000000000;
	Cmem[10][23:0] = 24'b00000000000000000000000;
	Cmem[11][23:0] = 24'b00000000000000000000000;
	Cmem[12][23:0] = 24'b00000000000000000000000;
	Cmem[13][23:0] = 24'b00000000000000000000000;
	Cmem[14][23:0] = 24'b00000000000000000000000;
	Cmem[15][23:0] = 24'b00000000000000000000000;
	
	extra[9:0] = 10'b0000000000;
	digit[2:0] = 2'b00;
	
	RamDataOut = 24'b00000000000000000000000;	
end

always@(Mode, RamAddress)begin

if(Mode == 0)begin
	
		if(RamAddress == 4'b0000)begin
			RamDataOut = Cmem[0];
		end
		if(RamAddress == 4'b0001)begin
			RamDataOut = Cmem[1];
		end
		if(RamAddress == 4'b0010)begin
			RamDataOut = Cmem[2];
		end
		if(RamAddress == 4'b0011)begin
			RamDataOut = Cmem[3];
		end
		if(RamAddress == 4'b0100)begin
			RamDataOut = Cmem[4];
		end
		if(RamAddress == 4'b0101)begin
			RamDataOut = Cmem[5];
		end
		if(RamAddress == 4'b0110)begin
			RamDataOut = Cmem[6];
		end
		if(RamAddress == 4'b0111)begin
			RamDataOut = Cmem[7];
		end
		if(RamAddress == 4'b1000)begin
			RamDataOut = Cmem[8];
		end
		if(RamAddress == 4'b1001)begin
			RamDataOut = Cmem[9];
		end
		if(RamAddress == 4'b1010)begin
			RamDataOut = Cmem[10];
		end
		if(RamAddress == 4'b1011)begin
			RamDataOut = Cmem[11];
		end
		if(RamAddress == 4'b1100)begin
			RamDataOut = Cmem[12];
		end
		if(RamAddress == 4'b1101)begin
			RamDataOut = Cmem[13];
		end
		if(RamAddress == 4'b1110)begin
			RamDataOut = Cmem[14];
		end
		if(RamAddress == 4'b1111)begin
			RamDataOut = Cmem[15];
		end		
	end
end

always @(posedge CLK) begin

if(Mode == 1)begin		//write op
			
			if(Op == 3'b000)begin					//and op
				Cmem[RamAddress][7:0] = RamDataIn[7:0] & Mask[7:0];
				Cmem[RamAddress][15:8] = RamDataIn[15:8] & Mask[7:0];
				Cmem[RamAddress][23:16] = RamDataIn[23:16] & Mask[7:0];
				
			end
			if(Op == 3'b001)begin					//or op
				Cmem[RamAddress][7:0] = RamDataIn[7:0] | Mask[7:0];
				Cmem[RamAddress][15:8] = RamDataIn[15:8] | Mask[7:0];
				Cmem[RamAddress][23:16] = RamDataIn[23:16] | Mask[7:0];
				
			end
			if(Op == 3'b010)begin					//xor op
				Cmem[RamAddress][7:0] = RamDataIn[7:0] ^ Mask[7:0];
				Cmem[RamAddress][15:8] = RamDataIn[15:8] ^ Mask[7:0];
				Cmem[RamAddress][23:16] = RamDataIn[23:16] ^ Mask[7:0];
				
			end
			if(Op == 3'b011)begin					//add op and check if 255
				extra[9:0] = RamDataIn[7:0] + Mask[7:0]; 
				
				if(extra[8] == 1'b1 || extra[9] == 1'b1)begin
					Cmem[RamAddress][7:0] = 8'b11111111;
				end
				else begin
					Cmem[RamAddress][7:0] = extra[7:0];
				end
				
				extra[9:0] = RamDataIn[15:8] + Mask[7:0]; 
				
				if(extra[8] == 1'b1 || extra[9] == 1'b1)begin
					Cmem[RamAddress][15:8] = 8'b11111111;
				end
				else begin
					Cmem[RamAddress][15:8] = extra[7:0];
				end

				extra[9:0] = RamDataIn[23:16] + Mask[7:0]; 
				
				if(extra[8] == 1'b1 || extra[9] == 1'b1)begin
					Cmem[RamAddress][23:16] = 8'b11111111;
				end
				else begin
					Cmem[RamAddress][23:16] = extra[7:0];
				end
				
			end
			if(Op == 3'b100)begin					//subtract and check if 0
				Cmem[RamAddress][7:0] = RamDataIn[7:0] - Mask[7:0];
				if(Mask[7:0] > RamDataIn[7:0])begin
					Cmem[RamAddress][7:0] = 8'b00000000;
				end
								
				Cmem[RamAddress][15:8] = RamDataIn[15:8] - Mask[7:0];
				if(Mask[7:0] > RamDataIn[15:8])begin
					Cmem[RamAddress][15:8] = 8'b00000000;
				end

				Cmem[RamAddress][23:16] = RamDataIn[23:16] - Mask[7:0];
				if(Mask[7:0] > RamDataIn[23:16])begin
					Cmem[RamAddress][23:16] = 8'b00000000;
				end
				
			end
			if(Op == 3'b101)begin					//increment and check 255
				
				extra[9:0] = RamDataIn[7:0] + 8'b00000001; 
				
				if(extra[8] == 1'b1 || extra[9] == 1'b1)begin
					Cmem[RamAddress][7:0] = 8'b11111111;
				end
				else begin
					Cmem[RamAddress][7:0] = extra[7:0];
				end
				
				extra[9:0] = RamDataIn[15:8] + 8'b00000001; 
				
				if(extra[8] == 1'b1 || extra[9] == 1'b1)begin
					Cmem[RamAddress][15:8] = 8'b11111111;
				end
				else begin
					Cmem[RamAddress][15:8] = extra[7:0];
				end

				extra[9:0] = RamDataIn[23:16] + 8'b00000001; 
				
				if(extra[8] == 1'b1 || extra[9] == 1'b1)begin
					Cmem[RamAddress][23:16] = 8'b11111111;
				end
				else begin
					Cmem[RamAddress][23:16] = extra[7:0];
				end
				
			end
			if(Op == 3'b110)begin					//decrement and check 0
				Cmem[RamAddress][7:0] = RamDataIn[7:0] - 8'b00000001;
				if(8'b00000001 > RamDataIn[7:0])begin
					Cmem[RamAddress][7:0] = 8'b00000000;
				end
								
				Cmem[RamAddress][15:8] = RamDataIn[15:8] - 8'b00000001;
				if(8'b00000001 > RamDataIn[15:8])begin
					Cmem[RamAddress][15:8] = 8'b00000000;
				end

				Cmem[RamAddress][23:16] = RamDataIn[23:16] - 8'b00000001;
				if(8'b00000001 > RamDataIn[23:16])begin
					Cmem[RamAddress][23:16] = 8'b00000000;
				end
				
			end
			if(Op == 3'b111)begin					//shift left and check 1st bit
				digit[0] = RamDataIn[7];
				Cmem[RamAddress][7:0] = RamDataIn[7:0] << 1;
				if(digit[0] == 1'b1)begin
					Cmem[RamAddress][0] = digit[0];
				end

				digit[0] = RamDataIn[15];
				Cmem[RamAddress][15:8] = RamDataIn[15:8] << 1;
				if(digit[0] == 1'b1)begin
					Cmem[RamAddress][8] = digit[0];
				end

				digit[0] = RamDataIn[23];
				Cmem[RamAddress][23:16] = RamDataIn[23:16] << 1;
				if(digit[0] == 1'b1)begin
					Cmem[RamAddress][16] = digit[0];
				end
				
			end	
	end
end

/*  Please write your code above  */
endmodule


module RgbMaskModule(input Mode, input [3:0] Address, input [23:0] RGBin,input [2:0] Op,  input CLK, output wire [23:0] RGBout);
	
	/*  DO NOT edit this module  */
	
	wire [7:0]  romResult;

	MaskROM RO(Address, romResult);
	RgbRAM RA(Mode, Address, RGBin,romResult, Op, CLK, RGBout);
endmodule