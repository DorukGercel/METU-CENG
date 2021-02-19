`timescale 1ns / 1ps

module lab4_2(
  input [1:0] mode,
  input [2:0] n_gasoline_pumps,      // for setup mode, max 6
  input [2:0] n_diesel_pumps,        // for setup mode, max 6
  input [3:0] fuel_amount,           // for add car mode, max 8
  input fuel_type,                   // for add car mode, gas(0) or diesel(1)
  input CLK, 

  output reg [0:5] pump_status,                // pump is working or not, max 6 pumps
  output reg is_gasoline_queue_not_full,       // gasoline queue full warning
  output reg is_diesel_queue_not_full,         // diesel queue full warning

  output reg [3:0] n_cars_in_gasoline_queue,   // to represent max 9 on 1 7-seg display
  output reg [3:0] n_cars_in_diesel_queue,     // to represent max 9 on 1 7-seg display

  output reg [7:0] total_gasoline_needed,      // to represent max 99 on 2 7-seg displays
  output reg [7:0] total_diesel_needed,        // to represent max 99 on 2 7-seg displays

  // reset these after one clock
  output reg invalid_gasoline_car,             // to lightup dot at an2
  output reg invalid_diesel_car,               // to lightup dot at an0
  output reg invalid_setup_params              // to lightup all dots
);

// write your code here

reg[3:0] gasoline_queue [0:7];
reg[3:0] diesel_queue [0:7];
reg[3:0] gas_station [0:5];
integer i;
integer j;
integer x;
reg flag;

initial begin
	flag = 1'b0;
	
	pump_status = 6'b000000;
  is_gasoline_queue_not_full = 1'b0;
  is_diesel_queue_not_full = 1'b0;
  n_cars_in_gasoline_queue = 4'b1111;
  n_cars_in_diesel_queue = 4'b1111;
  total_gasoline_needed = 8'b11111111;
  total_diesel_needed = 8'b11111111;
  invalid_gasoline_car = 1'b0;
  invalid_diesel_car = 1'b0;
  invalid_setup_params = 1'b0;
end


always@(posedge CLK)begin

if(mode[1] == 1'b1)begin
  pump_status = 6'b000000;
  is_gasoline_queue_not_full = 1'b0;
  is_diesel_queue_not_full = 1'b0;
  n_cars_in_gasoline_queue = 4'b1111;
  n_cars_in_diesel_queue = 4'b1111;
  total_gasoline_needed = 8'b11111111;
  total_diesel_needed = 8'b11111111;
  invalid_gasoline_car = 1'b0;
  invalid_diesel_car = 1'b0;
  invalid_setup_params = 1'b0;
  
  
  if((n_diesel_pumps + n_gasoline_pumps) > 3'b110 || (n_diesel_pumps + n_gasoline_pumps) == 3'b000)begin
    invalid_setup_params = 1'b1;
  end
  else begin
  
		for (i = 0;i < 8; i = i + 1) begin
      gasoline_queue[i][3:0] = 4'b0000;
      diesel_queue[i][3:0] = 4'b0000;
     end
    for(i = 0;i < 6; i = i + 1)begin
      gas_station[i] = 4'b0000;
    end

    invalid_gasoline_car = 1'b0;
    invalid_diesel_car = 1'b0;
    invalid_setup_params = 1'b0;
    
    if(n_gasoline_pumps != 3'b000)begin
      n_cars_in_gasoline_queue = 4'b0000;
      total_gasoline_needed = 8'b00000000;
		is_gasoline_queue_not_full = 1'b1;
    end
    if(n_diesel_pumps != 3'b000)begin 
      n_cars_in_diesel_queue = 4'b0000;
      total_diesel_needed = 8'b00000000;
		is_diesel_queue_not_full = 1'b1;
    end
    /*
    for(i = 0;i < 6; i = i + 1)begin
		if(i <  n_gasoline_pumps)begin
			pump_status[i] = 1'b1;
		end
    end*/
    for(i = 0;i < 6; i = i + 1)begin
		if(i <  n_gasoline_pumps + n_diesel_pumps)begin
			pump_status[i] = 1'b1;
		end
    end
	 flag = 1'b1;
  end
end

if(mode == 2'b00)begin
 if(flag == 1'b1) begin
  invalid_diesel_car = 1'b0;
  invalid_gasoline_car = 1'b0;
  invalid_setup_params = 1'b0;
  for(i = 0; i < 6; i = i + 1)begin                   //pump 1 gallon
    if(gas_station[i][3:0] > 4'b0000)begin
      gas_station[i][3:0] = gas_station[i][3:0] - 4'b0001;
    end
  end
  
  for(i = 0; i < 6; i = i + 1)begin
	if(i<n_gasoline_pumps)begin
    if(n_cars_in_gasoline_queue > 4'b0000)begin
      if(gas_station[i][3:0] == 4'b0000)begin
        gas_station[i][3:0] = gasoline_queue[0][3:0] + 4'b0001;     //find the right place to queue element
        pump_status[i] = 1'b0;

        for(x = 0; x < 7; x = x + 1)begin                 //arrange queue
          gasoline_queue[x][3:0] = gasoline_queue[x+1][3:0];
        end
        gasoline_queue[7] = 4'b0000;

        n_cars_in_gasoline_queue = n_cars_in_gasoline_queue - 4'b0001;
		  
		  is_gasoline_queue_not_full = 1'b1;

      end
	  end
	 end
  end

  for(j = 0; j < 6; j = j + 1)begin
	if(j >= n_gasoline_pumps && j<n_gasoline_pumps +n_diesel_pumps)begin
	 if(n_cars_in_diesel_queue > 4'b0000)begin
      if(gas_station[j][3:0] == 4'b0000)begin
        gas_station[j][3:0] = diesel_queue[0][3:0]  + 4'b0001;     //find the right place to queue element
        pump_status[j] = 1'b0;

        for(x = 0; x < 7; x = x + 1)begin                 //arrange queue
          diesel_queue[x][3:0] = diesel_queue[x+1][3:0];
        end
        diesel_queue[7] = 4'b0000;

        n_cars_in_diesel_queue = n_cars_in_diesel_queue - 4'b0001;
		  
		  is_diesel_queue_not_full = 1'b1;

      end
	 end
	end
  end
  
  
  if(n_gasoline_pumps > 3'b000)begin                               //sum gasoline in queue
	total_gasoline_needed = 8'b00000000;
    for(x = 0; x < 8; x = x + 1)begin
	  if(x <  n_cars_in_gasoline_queue)begin
      total_gasoline_needed = total_gasoline_needed + gasoline_queue[x][3:0];
	  end
    end  
  end

  for(i = 0; i < 6; i = i + 1)begin                  //sum gasoline in station
   if(i < n_gasoline_pumps)begin
	if(gas_station[i][3:0] > 4'b0001)begin
    total_gasoline_needed = total_gasoline_needed + gas_station[i][3:0] - 4'b0001;
	end
	end
  end

  if(n_diesel_pumps > 3'b000)begin                                 //sum diesel in queue
   total_diesel_needed = 8'b00000000;
    for(x = 0; x < 8; x = x + 1)begin
	  if(x < n_cars_in_diesel_queue)begin
      total_diesel_needed = total_diesel_needed + diesel_queue[x][3:0];
	  end
    end
  end

  for(j = 0; j < 6; j = j + 1)begin   //sum diesel in station
   if(j >= n_gasoline_pumps && j < n_gasoline_pumps + n_diesel_pumps)begin
   if(gas_station[j][3:0] > 4'b0001)begin
    total_diesel_needed = total_diesel_needed + gas_station[j][3:0]  - 4'b0001;
	end
	end
  end

 
  if(n_cars_in_gasoline_queue < 4'b1000)begin
    is_gasoline_queue_not_full = 1'b1;
  end
  if(n_cars_in_diesel_queue < 4'b1000)begin
    is_diesel_queue_not_full = 1'b1;
  end
  
  
  for(i = 0; i < 6; i = i + 1)begin                             //pump status is formed here
   if(i < n_gasoline_pumps + n_diesel_pumps)begin
    if(gas_station[i][3:0] > 4'b0001)begin
      pump_status[i] = 1'b0;
    end
    else begin
      pump_status[i] = 1'b1;
    end
	end
  end

  end
end


if(mode == 2'b01)begin
  if(flag == 1'b1)begin
  invalid_diesel_car = 1'b0;
  invalid_gasoline_car = 1'b0;
  invalid_setup_params = 1'b0;
	if(fuel_type == 1'b0)begin                  //we try to add gas car to queue
	  if(n_gasoline_pumps > 3'b000)begin
		if(fuel_amount > 4'b1000)begin            //invalid gas car
			invalid_gasoline_car = 1'b1;
		end
		else begin
      if(is_gasoline_queue_not_full == 1'b1)begin         //if queue not full add
        gasoline_queue[n_cars_in_gasoline_queue][3:0] = fuel_amount[3:0];
			  n_cars_in_gasoline_queue = n_cars_in_gasoline_queue + 1;
        if(n_cars_in_gasoline_queue == 4'b1000)begin
          is_gasoline_queue_not_full = 1'b0;
        end
      end
		else begin
			invalid_gasoline_car = 1'b1;
		end
		end
		end
		else begin
			invalid_gasoline_car = 1'b1;
		end
	end
  
  if(fuel_type == 1'b1)begin                  //we try to add diesel car to queue
		if(fuel_amount > 4'b1000 || n_diesel_pumps == 0)begin            //invalid diesel car
			invalid_diesel_car = 1'b1;
		end
		else begin
      if(is_diesel_queue_not_full == 1'b1)begin         //if queue not full add
        diesel_queue[n_cars_in_diesel_queue][3:0] = fuel_amount[3:0];
			  n_cars_in_diesel_queue = n_cars_in_diesel_queue + 1;
        if(n_cars_in_diesel_queue == 4'b1000)begin
          is_diesel_queue_not_full = 1'b0;
        end
      end
		else begin
			invalid_diesel_car = 1'b1;
		end
		end
	end

  for(i = 0; i < 6; i = i + 1)begin                   //pump 1 gallon
    if(gas_station[i][3:0] > 4'b0000)begin
      gas_station[i][3:0] = gas_station[i][3:0] - 4'b0001;
    end
  end
  
  for(i = 0; i < 6; i = i + 1)begin
   if(i<n_gasoline_pumps)begin
    if(n_cars_in_gasoline_queue > 4'b0000)begin
      if(gas_station[i][3:0] == 4'b0000)begin
        gas_station[i][3:0] = gasoline_queue[0][3:0] + 4'b0001;     //find the right place to queue element
        pump_status[i] = 1'b0;

        for(x = 0; x < 7; x = x + 1)begin                 //arrange queue
          gasoline_queue[x][3:0] = gasoline_queue[x+1][3:0];
        end
        gasoline_queue[7] = 4'b0000;

        n_cars_in_gasoline_queue = n_cars_in_gasoline_queue - 4'b0001;
		  
		  is_gasoline_queue_not_full = 1'b1;

      end
	  end
	 end
  end

  for(j = 0; j < 6; j = j + 1)begin
   if(j >=n_gasoline_pumps && j < n_gasoline_pumps + n_diesel_pumps)begin
	 if(n_cars_in_diesel_queue > 4'b0000)begin
      if(gas_station[j][3:0] == 4'b0000)begin
        gas_station[j][3:0] = diesel_queue[0][3:0]  + 4'b0001;     //find the right place to queue element
        pump_status[j] = 1'b0;

        for(x = 0; x < 7; x = x + 1)begin                 //arrange queue
          diesel_queue[x][3:0] = diesel_queue[x+1][3:0];
        end
        diesel_queue[7] = 4'b0000;

        n_cars_in_diesel_queue = n_cars_in_diesel_queue - 4'b0001;
		  
		  is_diesel_queue_not_full = 1'b1;

      end
	 end
	end
  end
  
  
  if(n_gasoline_pumps > 3'b000)begin                               //sum gasoline in queue
	total_gasoline_needed = 8'b00000000;
    for(x = 0; x < 8; x = x + 1)begin
	  if(x < n_cars_in_gasoline_queue)begin
      total_gasoline_needed = total_gasoline_needed + gasoline_queue[x][3:0];
	  end
    end  
  end

  for(i = 0; i < 6; i = i + 1)begin                  //sum gasoline in station
   if(i < n_gasoline_pumps)begin
	if(gas_station[i][3:0] > 4'b0001)begin
    total_gasoline_needed = total_gasoline_needed + gas_station[i][3:0] - 4'b0001;
	end
	end
  end

  if(n_diesel_pumps > 3'b000)begin                                 //sum diesel in queue
   total_diesel_needed = 8'b00000000;
    for(x = 0; x < 8; x = x + 1)begin
	  if(x < n_cars_in_diesel_queue)begin
      total_diesel_needed = total_diesel_needed + diesel_queue[x][3:0];
	  end
    end
  end

  for(j = 0; j < 6; j = j + 1)begin   //sum diesel in station
   if(j >= n_gasoline_pumps && j< n_gasoline_pumps + n_diesel_pumps)begin
   if(gas_station[j][3:0] > 4'b0001)begin
    total_diesel_needed = total_diesel_needed + gas_station[j][3:0]  - 4'b0001;
	end
	end
  end

 
  
  if(n_cars_in_gasoline_queue < 4'b1000)begin
    is_gasoline_queue_not_full = 1'b1;
  end
  if(n_cars_in_diesel_queue < 4'b1000)begin
    is_diesel_queue_not_full = 1'b1;
  end
  
  
  for(i = 0; i < 6; i = i + 1)begin                             //pump status is formed here
   if(i < n_gasoline_pumps + n_diesel_pumps)begin
    if(gas_station[i][3:0] > 4'b0001)begin
      pump_status[i] = 1'b0;
    end
    else begin
      pump_status[i] = 1'b1;
    end
	end
  end 


  end	
  
end

end

endmodule
