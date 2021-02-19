:- module(hw5,[catomic_number/2,ion/2,molecule/2]).
:-[catoms].

sum_list([], 0).
sum_list([H|T], Sum) :-
   sum_list(T, Rest),
   Sum is H + Rest.

catomic_number(X, Y) :-
    catom(X, _, _, Z), sum_list(Z, Sum), Y is Sum.


last([X], X).
last([_|Tail], X) :- last(Tail, X).

ion(Name, Charge):-
    catom(Name,_,_,List), last(List, Y),valence(Y, Charge).

valence(X, Val):-
    X =< 4 -> Val is X ; Val is X-8.  


molecule(Catom_List, Total_Catomic_Number):-
    no_summer(Catom_List, 0, Total_Catomic_Number, 0, 0).

no_summer([H1], No, T, Ion, A):-
    catomic_number(H1,X),
    NoR is X + No,
    NoR =:= T,
    ion(H1,Z),
    IonR is Ion + Z,
    A =:= IonR.
%no_summer([], No ,T, Ion, A):-
%    No =:= T,
%    Ion =:= A.
no_summer([H1,H2|R], No, T, Ion, A):-
    catomic_number(H1,X), catomic_number(H2, Y),
    X =< Y,
    NoR is X + No,
    NoR < T,
    ion(H1, Z),
    IonR is Ion + Z,
    no_summer([H2|R], NoR, T, IonR, A).
%ion_summer([], 0).
%ion_summer([H|R], Ion):-
%    ion(H, X), ion_summer(R, IRest), Ion is X + IRest.


%smaller_list([]).
%smaller_list([_]).
%smaller_list([H|Rest]):-
%    first_elem(Rest, X), catomic_number(H,A), catomic_number(X, B),
%     min(A, B, A),smaller_list(Rest).

%min(A,B, X):-
%    A =< B -> X is A; X is B.

%first_elem([H], H).
%first_elem([H|_], H).

%equal(A,A).