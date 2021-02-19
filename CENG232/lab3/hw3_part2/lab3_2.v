`timescale 1ns / 1ps 
module lab3_2(
			input[3:0] number,
			input CLK, 
			input selection,
			input mode,
			input clear,
			output reg [7:0] digit1,
			output reg [7:0] digit0,
			output reg [7:0] count1,
			output reg [7:0] count0,
			output reg warning
    );
   //Modify the lines below to implement your design .
	integer prime_count1;
	integer prime_count0;
	integer norm_count1;
	integer norm_count0;
	
	//((~number[3]&&~number[2]&&number[1]) + (~number[3]&& number[2]&&number[0]) + (~number[2]&& number[0]&&number[1]) + (~number[0]&&number[2]&&number[1]))
	
	initial begin
		digit1 = 0;
		digit0 = 0;
		count1 = 0;
		count0 = 0;
		warning = 0;
		prime_count1 = 0;
	   prime_count0 = 0;
	   norm_count1 = 0;
		norm_count0 = 0;
	end
	
	always@(posedge CLK)
	
	begin
		if(clear==1)begin
			digit1 = 0;
			digit0 = 0;
			count1 = 0;
			count0 = 0;
			warning = 0;
			prime_count1 = 0;
			prime_count0 = 0;
			norm_count1 = 0;
			norm_count0 = 0;
		end
		else begin
		if(((number == 4'b0010) || (number == 4'b0011) || (number == 4'b0101) || (number == 4'b0111) || (number == 4'b1011) || (number == 4'b1101)) && (selection == 0))begin
			if(mode == 0)begin
				if(number == 4'b0010)begin
					digit1 = 4'b1101;
					digit0 = number;
					prime_count0 = prime_count0 + 1;
					if(prime_count0 == 8'd10) prime_count0 = 0;
					count1 = prime_count1;
					count0 = prime_count0 ;
					warning = 0;
				end
				else if(number == 4'b0011)begin
					digit1 = 4'b0010;
					digit0 = number;
					prime_count0 = prime_count0 + 1;
					if (prime_count0 == 8'd10) prime_count0 = 0;
					count1 = prime_count1;
					count0 = prime_count0;
					warning = 0;
				end
				else if(number == 4'b0101)begin
					digit1 = 4'b0011;
					digit0 = number;
					prime_count0 = prime_count0 + 1;
					if (prime_count0 == 8'd10) prime_count0 = 0;
					count1 = prime_count1;
					count0 = prime_count0;
					warning = 0;
				end
				else if(number == 4'b0111)begin
					digit1 = 4'b0101;
					digit0 = number;
					prime_count0 = prime_count0 + 1;
					if (prime_count0 == 8'd10) prime_count0 = 0;
					count1 = prime_count1;
					count0 = prime_count0;
					warning = 0;
				end
				else if(number == 4'b1011)begin
					digit1 = 4'b0111;
					digit0 = number;
					prime_count0 = prime_count0 + 1;
					if (prime_count0 == 8'd10) prime_count0 = 0;
					count1 = prime_count1;
					count0 = prime_count0;
					warning = 0;
				end
				else if(number == 4'b1101)begin
					digit1 = 4'b1011;
					digit0 = number;
					prime_count0 = prime_count0 + 1;
					if (prime_count0 == 8'd10) prime_count0 = 0;
					count1 = prime_count1;
					count0 = prime_count0;
					warning = 0;
				end
			end
			else if(mode == 1)begin
				if(number == 4'b0010)begin
					digit1 = 4'b0011;
					digit0 = number;
					prime_count1 = prime_count1 + 1;
					if (prime_count1 == 8'd10) prime_count1 = 0;
					count1 = prime_count1;
					count0 = prime_count0;
					warning = 0;
				end
				else if(number == 4'b0011)begin
					digit1 = 4'b0101;
					digit0 = number;
					prime_count1 = prime_count1 + 1;
					if (prime_count1 == 8'd10) prime_count1 = 0;
					count1 = prime_count1;
					count0 = prime_count0;
					warning = 0;
				end
				else if(number == 4'b0101)begin
					digit1 = 4'b0111;
					digit0 = number;
					prime_count1 = prime_count1 + 1;
					if (prime_count1 == 8'd10) prime_count1 = 0;
					count1 = prime_count1;
					count0 = prime_count0;
					warning = 0;
				end
				else if(number == 4'b0111)begin
					digit1 = 4'b1011;
					digit0 = number;
					prime_count1 = prime_count1 + 1;
					if (prime_count1 == 8'd10) prime_count1 = 0;
					count1 = prime_count1;
					count0 = prime_count0;
					warning = 0;
				end
				else if(number == 4'b1011)begin
					digit1 = 4'b1101;
					digit0 = number;
					prime_count1 = prime_count1 + 1;
					if (prime_count1 == 8'd10) prime_count1 = 0;
					count1 = prime_count1;
					count0 = prime_count0;
					warning = 0;
				end
				else if(number == 4'b1101)begin
					digit1 = 4'b0010;
					digit0 = number;
					prime_count1 = prime_count1 + 1;
					if (prime_count1 == 8'd10) prime_count1 = 0;
					count1 = prime_count1;
					count0 = prime_count0;
					warning = 0;
				end
			end
		end
		else if(!(number == 4'b0010 || number == 4'b0011 || number == 4'b0101 || number == 4'b0111 || number == 4'b1011 || number == 4'b1101) && (selection == 1))begin
			if(mode == 0)begin
				if(number == 4'b0000)begin
					digit1 = 4'b0000;
					digit0 = number;
					norm_count0 = norm_count0 + 1;
					if (norm_count0 == 8'd10) norm_count0 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b0001)begin
					digit1 = 4'b0000;
					digit0 = number;
					norm_count0 = norm_count0 + 1;
					if (norm_count0 == 8'd10) norm_count0 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b0100)begin
					digit1 = 4'b0010;
					digit0 = number;
					norm_count0 = norm_count0 + 1;
					if (norm_count0 == 8'd10) norm_count0 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b0110)begin
					digit1 = 4'b0011;
					digit0 = number;
					norm_count0 = norm_count0 + 1;
					if (norm_count0 == 8'd10) norm_count0 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b1000)begin
					digit1 = 4'b0100;
					digit0 = number;
					norm_count0 = norm_count0 + 1;
					if (norm_count0 == 8'd10) norm_count0 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b1001)begin
					digit1 = 4'b0100;
					digit0 = number;
					norm_count0 = norm_count0 + 1;
					if (norm_count0 == 8'd10) norm_count0 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b1010)begin
					digit1 = 4'b0101;
					digit0 = number;
					norm_count0 = norm_count0 + 1;
					if (norm_count0 == 8'd10) norm_count0 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b1100)begin
					digit1 = 4'b0110;
					digit0 = number;
					norm_count0 = norm_count0 + 1;
					if (norm_count0 == 8'd10) norm_count0 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b1110)begin
					digit1 = 4'b0111;
					digit0 = number;
					norm_count0 = norm_count0 + 1;
					if (norm_count0 == 8'd10) norm_count0 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b1111)begin
					digit1 = 4'b0111;
					digit0 = number;
					norm_count0 = norm_count0 + 1;
					if (norm_count0 == 8'd10) norm_count0 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
			end
			if(mode == 1)begin
				if(number == 4'b0000)begin
					digit1 = 4'b0000;
					digit0 = number;
					norm_count1 = norm_count1 + 1;
					if (norm_count1 == 8'd10) norm_count1 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b0001)begin
					digit1 = 4'b0010;
					digit0 = number;
					norm_count1 = norm_count1 + 1;
					if (norm_count1 == 8'd10) norm_count1 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
				end
				else if(number == 4'b0100)begin
					digit1 = 4'b1000;
					digit0 = number;
					norm_count1 = norm_count1 + 1;
					if (norm_count1 == 8'd10) norm_count1 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b0110)begin
					digit1 = 4'b1100;
					digit0 = number;
					norm_count1 = norm_count1 + 1;
					if (norm_count1 == 8'd10) norm_count1 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b1000)begin
					digit1 = 4'b0000;
					digit0 = number;
					norm_count1 = norm_count1 + 1;
					if (norm_count1 == 8'd10) norm_count1 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b1001)begin
					digit1 = 4'b0010;
					digit0 = number;
					norm_count1 = norm_count1 + 1;
					if (norm_count1 == 8'd10) norm_count1 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b1010)begin
					digit1 = 4'b0100;
					digit0 = number;
					norm_count1 = norm_count1 + 1;
					if (norm_count1 == 8'd10) norm_count1 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b1100)begin
					digit1 = 4'b1000;
					digit0 = number;
					norm_count1 = norm_count1 + 1;
					if (norm_count1 == 8'd10) norm_count1 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b1110)begin
					digit1 = 4'b1100;
					digit0 = number;
					norm_count1 = norm_count1 + 1;
					if (norm_count1 == 8'd10) norm_count1 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
				else if(number == 4'b1111)begin
					digit1 = 4'b1110;
					digit0 = number;
					norm_count1 = norm_count1 + 1;
					if (norm_count1 == 8'd10) norm_count1 = 0;
					count1 = norm_count1;
					count0 = norm_count0;
					warning = 0;
				end
			end
		end
		else if((!(number == 4'b0010 || number == 4'b0011 || number == 4'b0101 || number == 4'b0111 || number == 4'b1011 || number == 4'b1101) && (selection == 0)) || ((number == 4'b0010 || number == 4'b0011 || number == 4'b0101 || number == 4'b0111 || number == 4'b1011 || number == 4'b1101) && (selection == 1)))begin
			warning = 1;
		end
		end
	end

endmodule


