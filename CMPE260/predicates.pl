% This function finds all of the teams and adds them to list A, also adds permutations of A to L.
allTeams(L,N):- findall(X,team(X,_),A), 
		permutation(A,L), 
		length(L,N).

% This function uses findall to find teams that has been defeated by the team t up to a week W.
% S1 and S4 are the goals scored by t. 
% S2 and S3 are the goals scored by opponent teams.
% L1 is the list of teams that has been defeated by team t away up to a week W.
% L2 is the list of teams that has been defeated by team t home up to a week W.
% L is L1 and L2 appended.

wins(T,W,L,N):- findall(X,(match(K, T, S1, X, S2),K=<W,S1>S2),L1), 
		findall(Y,(match(N, Y, S3, T, S4),N=<W,S4>S3),L2),
		append(L1,L2,L), length(L,N).

% This function uses findall to find teams that t has been defeated by up to a week W.
% S1 and S4 are the goals scored by t. 
% S2 and S3 are the goals scored by opponent teams.
% L1 is the list of teams that t has been defeated by home up to a week W.
% L2 is the list of teams that t has been defeated by away up to a week W.
% L is L1 and L2 appended.

losses(T,W,L,N):- findall(X,(match(K, T, S1, X, S2),K=<W,S1<S2),L1), 
		findall(Y,(match(N, Y, S3, T, S4),N=<W,S4<S3),L2),
		append(L1,L2,L), length(L,N).

% This function uses findall to find teams that has been tied with team t up to a week W.
% S1 and S4 are the goals scored by t. 
% S2 and S3 are the goals scored by opponent teams.
% L1 is the list of teams that has been tied with the team t away up to a week W.
% L2 is the list of teams that has been tied with the team t home up to a week W.
% L is L1 and L2 appended.

draws(T,W,L,N):- findall(X,(match(K, T, S1, X, S2),K=<W,S1=S2),L1), 
		findall(Y,(match(N, Y, S3, T, S4),N=<W,S4=S3),L2),
		append(L1,L2,L), length(L,N).

% This function returns the sum of a list.
% If the list is empty, it returns 0.
% Else it separates the head from the list and adds it to the sum.

sum_list([], 0).
sum_list([H|T], Sum) :- sum_list(T, Rest), Sum is H + Rest.

% This function uses findall to find all of the goals scored by team t in all matches up to a week w.
% K and N are the numbers of weeks.
% X and Y  are the goals scored by team t.
% L is L1 and L2 appended.
% S is the sum of the elements in L.

scored(T,W,S):-  findall(X,(match(K,T,X,,),K=<W),L1),
		findall(Y,(match(N,,,T,Y),N=<W),L2),
		append(L1,L2,L),
		sum_list(L,S).

% This function uses findall to find all of the goals conceded by team t in all matches up to a week w.
% K and N are the numbers of weeks.
% X and Y  are the goals conceded by team t.
% L is L1 and L2 appended.		
% S is the sum of the elements in L.

conceded(T,W,S):-  findall(X,(match(K,T,,,X),K=<W),L1),
		findall(Y,(match(N,,Y,T,),N=<W),L2),
		append(L1,L2,L),
		sum_list(L,S).

% This function finds the a teams average using scored and conceded.
% A1 is the number of goals that has been scored by team t up to a week W.
% A2 is the number of goals that has been conceded by team t up to a week W.
% A is the substraction of A1 and A2.

average(T,W,A):- scored(T,W,A1), conceded(T,W,A2), A is A1-A2.

% This functions orders all teams in terms of their averages using a instertion sort algorithm.
% L1 is all of the teams.
% L is L1 sorted using insert_sort function.

order(L,W):- allTeams(L1,N), insert_sort(L1,L,W).

% This function sorts a given list in terms of their averages using the logic of insertion sort, recursively.
% It returns the sorted list if the given list is empty.
% ELse, it compares the head of the list with the other lists head, if it is smaller, class the same function with the tail of the list, of it is bigger stops.
% It does this procedure for every element of the list recursively.

insert_sort(List,Sorted,W):- i_sort(List,[],Sorted,W).
i_sort([],Acc,Acc,W).
i_sort([H|T],Acc,Sorted,W):-insert(H,Acc,NAcc,W),i_sort(T,NAcc,Sorted,W).
   
insert(X,[Y|T],[Y|NT],W):- average(X,W,X1),average(Y,W,Y1),X1=<Y1,insert(X,T,NT,W).
insert(X,[Y|T],[X,Y|T],W):- average(X,W,X1),average(Y,W,Y1),X1>Y1.
insert(X,[],[X],W).


% This function returns the first three teams in the ordered list up to a week W.
% L1 is the ordered teams.
% L is the list of first three teams that obtained using first_elements function.

topThree(L,W):- order(L1,W), first_elements(3,L1,L).


% This function returns the first N elements of the given list.
% If the given N is 0, it returns an empty list.
% Else it adds the head of the list to list L and decrements N and calls it N1, calls itself again with N1.

first_elements(0,_,[]).
first_elements(N,[H|T],[H|L1]):- N1 is N-1, first_elements(N1,T,L1).