N = 1503; 			%size of Monte Carlo
lambda = 160; 			%lambda value for Poisson variable
p = 0.79;			%probability of success
Total_Triangles = zeros(N,1);   %Each triangles in each simulation
All_Ratios = zeros(N,1);  %Each ratios in each simulation

for k=1:N;			%Each simulation loop

	Poisson_X = poissrnd(lambda);		%No of distinct good types processed within a day
	Matrix_G = zeros(Poisson_X,Poisson_X);	%Creating the zero matrix according to Poisson_X

	for i=1:Poisson_X;		%Fill the adjacency matrix
		for j = 1:i-1;
			U = rand;	%Bernoulli Success Condition
			if(U < p)		
				Matrix_G(i,j) = 1;
			endif;
 	
		endfor;
	endfor;
  
	Matrix_G = Matrix_G + Matrix_G';	%Form the Symmetry Matrix

	No_Triangles = sum(diag(Matrix_G ^ 3))/6;	%No of triangles

	No_Triplets = nchoosek(Poisson_X, 3);		%No of triplets

	Ratio = No_Triangles/No_Triplets;	%Wanted Ratio
  
  	Total_Triangles(k) = No_Triangles;    %No of triangles for this simulation

	All_Ratios(k) = Ratio;		%The ratio of triangles to the triplets 
endfor;

answer_b = mean(All_Ratios > 0.5);		%Probability that at most 1 distinct choice can be made

answer_c_b_X = mean(Total_Triangles);     %Estimated number of potential shipments (X value)

answer_c_b_Y = mean(All_Ratios);        %Estimated ratio (Y value)

answer_d_b_X = std(Total_Triangles);    %Standard deviaton of X

answer_d_b_Y = std(All_Ratios);       %Standard deviation of Y 

display(answer_b);
display(answer_c_b_X);
display(answer_c_b_Y);
display(answer_d_b_X);
display(answer_d_b_Y);
