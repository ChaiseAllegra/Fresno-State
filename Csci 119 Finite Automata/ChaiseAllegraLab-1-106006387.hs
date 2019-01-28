    ---- CSci 119, Lab 1 ----

	-- Note: you will replace all instances of "undefined" below with your answers.


	---- Boolean operators

	-- The following code tests whether "and" is commutative:
	bools = [True, False]
	and_commutes = and [(p && q) == (q && p) | p <- bools, q <- bools]

	-- Write similar definitions that test whether "or" is commutative,
	-- "and" and "or" are associative, "and" distributes over "or",
	-- "or" distributes over "and"
	
	--Chaise Allegra Lab-1- 106006387
	
	or_commutes = and [(p || q) == (q || p) | p <- bools, q <- bools]
	and_assoc = and [((p && q) && r) == (p && (q && r)) | p <- bools, q <- bools, r <- bools]
	or_assoc = and [((p || q) || r) == (p || (q || r)) | p <- bools, q <- bools, r <- bools]
	and_dist = and [(p && (q || r)) == ((p && q) || (p && r)) | p <- bools, q <- bools, r <- bools]
	or_dist = and [(p || (q && r)) == ((p || q) && (p || r)) | p <- bools, q <- bools, r <- bools]

			  
	-- The exclusive-or operation on Bool in Haskell is equivalent to /=.
	-- Test the properties of this operation (commutativity, associativity,
	-- distributivity over and+or, and distributivity of and+or over it)
	-- using the same method as above
	xor_commutes = and [(p /= q) == (q /= p) | p <- bools, q <- bools]
		--true
	xor_assoc = and [((p /= q) /= x) == (p /= (q /= x)) | p <- bools, q <- bools, x <- bools]
		--true
	xor_dist_and = and [(p /= (q && x)) == ((p /= q) && (p /= x)) | p <- bools, q <- bools, x <- bools]
		--false
	xor_dist_or = and [(p /= (q || x)) == ((p /= q) || (p /= x)) | p <- bools, q <- bools, x <- bools]
		--true
	and_dist_xor = and [(p && (q /= x)) == ((p && q) /= (p && x)) | p <- bools, q <- bools, x <- bools]
			--true
	or_dist_xor = and [(p || (q /= x)) == ((p || q) /= (p || x)) | p <- bools, q <- bools, x <- bools]
			--false
	-- The implication operator on Bool in Haskell is equivalent to <=.
	-- Check whether implication is associative or commutative:
	assoc_imp = and [(p<=q)&&x == ((q<=x)&&p) | p <- bools, q <- bools,x <- bools]--false
	comm_imp = and [(p<=q) == ((q<=p)) | p <- bools, q <- bools]--false


	----- Evaluating statements involving quantifiers

	-- Assume that the universe of discourse is the set {1,2,3,4,5,6,7,8},
	-- that is, that the word "number" temporarily means 1, 2, ..., 8.

	u = [1..8]

	-- Translate each of the following statements first, in a comment, into a
	-- logical statement involving forall, exists, and, or, imp, and not,
	-- and then into Haskell code that checks ("brute force") whether
	-- the statement is true. I'll work one example.

	-- 1. "Every number that's greater than 2 is greater than 1"
	-- A: either  (n > 2) or (n > 1)
	--prob1= and [(n > 2) <= (n > 1) | n <- u]
	prob1 = and [(n > 2) <= (n > 1)|n<-u]

	-- 2. Every number is either greater than 1 or less than 2
	-- A: either (n>1) or (n<2)
	prob2 = and [(n > 1) || (n < 2) | n <- u]

	-- 3. Every two numbers are comparable with <= (i.e., either one is <=
	--    the other or vice-versa)
	-- A: either n<=x or x<=n
	prob3 = and [n<=x||x<=n | n <- u, x <-u]
	--does it compare all possible numebrs of u

	-- 4. For every odd number, there is a greater even number (use the Haskell
	--    predicates odd, even :: Integral a => a -> Bool)
	-- A: for every (odd n), there is(even x) and (x > n)
	prob4 = and [or [n<x |x<-u,even x ] | n <- u, odd n ]

	-- 5. For every even number, there is a greater odd number
	-- A: for every (even n), there is a (odd x) and (x > n)
	prob5 =  and [or[x > n | x <- u,odd x] | n <- u, even n] 

	-- 6. There are two odd numbers that add up to 6
	-- A: there is n, x, (odd n) and (odd x) and n+x==6
	prob6 = or [n + x == 6 | n <- u, odd n, x <- u, odd x]

	-- 7. There is a number that is at least as large as every number
	--    (i.e., according to >=)
	-- A: there is n, forall x, where n >= x
	prob7 = or [and[x<=n| x <- u]|n <- u]
				

	-- 8. For every number, there is a different number such that there are no
	--    numbers between these two.
	-- A: for every n, there is a x, such that (n /= x) 
	prob8 = and [or[and[not(z<x&&z>n)|z<-u]|x <- u, x /= n]| n <- u]
    
    