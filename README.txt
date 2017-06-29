From the input of a premise and conclusion, this program will build a truth table and determine whether the corresponding logic formula is a tautology.

Valid commands (case-sensitive): 
   ~ (negation)
   V (or)
   ^ (and)
   -> (implies)
   <-> (iff)
   XOR (exclusive or)
   t (true statement)
   f (false statement)
   Parentheses also allowed.

You may use any character not defined above as a variable.

EXAMPLE:

Example premise input: p V (q -> z)
Example conclusion input: z <-> ~p

Output will be:

p q z | p V (q -> z) | z <-> ~p
t t t |      t       |    f     Premise does not imply conclusion in this case.
f t t |      t       |    t    
t f t |      t       |    f     Premise does not imply conclusion in this case.
f f t |      t       |    t    
t t f |      t       |    t    
f t f |      f       |    f    
t f f |      t       |    t    
f f f |      t       |    f     Premise does not imply conclusion in this case.

The statement [(p V (q -> z)) -> (z <-> ~p)]
is not a tautology. Invalid cases are labeled.

