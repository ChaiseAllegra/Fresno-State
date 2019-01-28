-- Lab 7: Convert FSMs to regular expressions

import Data.List (sort, elemIndex)
import Data.Char (intToDigit)

---------------- Given functions ----------------

-- normalize a list, i.e., sort and remove (now adjacent) duplicates
norm :: Ord a => [a] -> [a]
norm = mynub . sort where
  mynub [] = []
  mynub [x] = [x]
  mynub (x:ys@(y:zs)) | x == y = mynub ys
                      | otherwise = x : mynub ys

-- Ordinary regular expressions and a show method for them
data RE  = Empty | Letter Char | Union RE RE | Cat RE RE | Star RE
instance (Show RE) where    -- use precedence to minimize parentheses
  showsPrec d Empty         = showString "@"
  showsPrec d (Letter c)    = showString [c]
  showsPrec d (Union r1 r2) = showParen (d > 6) $  -- prec(Union) = 6
                              showsPrec 6 r1 .
                              showString "+" .
                              showsPrec 6 r2
  showsPrec d (Cat r1 r2)   = showParen (d > 7) $  -- prec(Cat) = 7
                              showsPrec 7 r1 .
                              showsPrec 7 r2
  showsPrec d (Star r1)     = showsPrec 9 r1 .     -- prec(Star) = 8
                              showString "*"

-- Extended regular expressions, including a name for One = Star Empty,
-- and arbitrary numbers of arguments for (associative) Union and Cat
data RE' = Zero | One | Letter' Char | Union' [RE'] | Cat' [RE'] | Star' RE'
  deriving (Eq, Ord, Show)

-- Convert ordinary REs into extended REs
toRE' :: RE -> RE'
toRE' Empty = Zero
toRE' (Letter c) = Letter' c
toRE' (Union r1 r2) = Union' [toRE' r1, toRE' r2]
toRE' (Cat r1 r2) = Cat' [toRE' r1, toRE' r2]
toRE' (Star r1) = Star' (toRE' r1)

-- Convert extended REs into ordinary REs, eliminating Union' and Cat' on
-- lists of length < 2, and right-associating them on longer lists
toRE :: RE' -> RE
toRE Zero = Empty
toRE One = Star Empty
toRE (Letter' c) = Letter c
toRE (Union' []) = Empty
toRE (Union' [r]) = toRE r
toRE (Union' (r:rs)) = Union (toRE r) (toRE (Union' rs))
toRE (Cat' []) = Star Empty
toRE (Cat' [r]) = toRE r
toRE (Cat' (r:rs)) = Cat (toRE r) (toRE (Cat' rs))
toRE (Star' r) = Star (toRE r)

-- Basic postfix parser for RE', assuming binary + and ., for testing
re :: String -> RE'
re w = re' w [] where
  re' [] [r] = r
  re' ('0':xs) rs = re' xs (Zero:rs)
  re' ('1':xs) rs = re' xs (One:rs)
  re' ('+':xs) (r2:r1:rs) = re' xs (Union' [r1,r2]:rs)
  re' ('.':xs) (r2:r1:rs) = re' xs (Cat' [r1,r2]:rs)
  re' ('*':xs) (r:rs) = re' xs (Star' r:rs)
  re' (x:xs) rs = re' xs (Letter' x:rs)


-- Finite state machines (as records), indexed by the type of their states
-- M = FSM {states=qs, start=s, finals=fs, delta=d}
data FSM a = FSM {
  states :: [a],
  start  :: a,
  finals :: [a],
  delta  :: [(a,Char,a)]
  }


-- ap ts q a == the unique q' such that (q, a, q') is in ts;  assumes success
ap :: Eq a => [(a,Char,a)] -> a -> Char -> a 
ap ((q1, a1, q2):ts) q a | q1 == q && a1 == a = q2
                         | otherwise = ap ts q a


---------------- Lab 7 begins here ----------------

sigma = "ab"  -- Everything below should work with any choice of sigma


-------- Part 1

-- Reimplement your RE operations from Part 1 of Lab 4 for the type RE'
emptiness :: RE' -> Bool
emptiness (Zero) = True 
emptiness (Letter' a)= False 
emptiness(One)= False
emptiness(Union' [])=undefined
emptiness(Union' [r])=emptiness(r)
emptiness (Union' (r1:rs))=emptiness(r1)&&emptiness(Union' rs)
emptiness(Cat' []) = undefined
emptiness(Cat' [r]) = emptiness(r)
emptiness (Cat' (r1:rs))=emptiness(r1)||emptiness(Cat' rs)
emptiness(Star' r)=False 

unitarity:: RE'->Bool
unitarity (Zero)= False 
unitarity(One)= True
unitarity(Letter' a)= False 
unitarity(Union' [])=undefined
unitarity(Union' [r])=unitarity(r)
unitarity(Union' (r1:rs))=unitarity(r1)&&emptiness(Union' rs)||unitarity(Union' rs)&&emptiness(r1)||unitarity(r1)&&unitarity(Union' rs)
unitarity(Cat' [])=undefined
unitarity(Cat' [r])=unitarity(r)
unitarity (Cat' (r1:rs))=unitarity(r1)&&unitarity(Cat' rs)
unitarity(Star' r1)=emptiness(r1)||unitarity(r1) 

bypassability::RE'->Bool
bypassability (Zero)= False 
bypassability(One)=True
bypassability(Letter' a)= False 
bypassability(Union' [])=undefined
bypassability(Union' [r])=bypassability(r)
bypassability(Union' (r1:rs))=bypassability(r1)||bypassability(Union' rs) 
bypassability(Cat' [])=undefined
bypassability(Cat' [r])=bypassability(r)
bypassability(Cat' (r1:rs))=bypassability(r1)&&bypassability(Cat' rs) 
bypassability(Star' r1)=True 

infiniteness::RE'->Bool
infiniteness(Zero)= False 
infiniteness(One)=False
infiniteness(Letter' a)= False
infiniteness(Union' [])=undefined
infiniteness(Union' [r])=infiniteness(r)
infiniteness(Union' (r1:rs))=infiniteness(r1)||infiniteness(Union' rs)
infiniteness(Cat' [])=undefined
infiniteness(Cat' [r])=infiniteness(r)
infiniteness(Cat' (r1:rs))=(infiniteness(r1)&&not(emptiness(Cat' rs)))||(infiniteness(Cat' rs)&&not(emptiness(r1)))
infiniteness(Star' r1)=not(emptiness(r1))&&not(unitarity(r1))

reversal::RE'->RE'
reversal(Zero)= Zero 
reversal(One)=One
reversal(Letter' a)= Letter' a 
reversal(Union' [])=undefined
reversal(Union' [r])=reversal(r)
reversal(Union' (r1:rs))= Union'([(reversal(Union' rs)),(reversal(r1))])
reversal(Cat' [r])=reversal(r)
reversal(Cat' (r1:rs))= (Cat' [(reversal(Cat' rs)), (reversal(r1))])
reversal(Star' r1)=Star'(reversal(r1))


leftQ::RE'->Char->RE'
leftQ(Zero) w= Zero 
leftQ(One) w=Zero
leftQ(Letter' a) w | a==w = Star' (Zero) 
                   | a/=w = Zero
leftQ(Union' [] )w=undefined
leftQ(Union' [r])w=leftQ(r)w
leftQ(Union' (r1:rs)) w=Union' ([(leftQ (r1) w), (leftQ (Union' rs) w)]) 
leftQ(Cat' []) w=undefined
leftQ(Cat' [r])w=leftQ(r)w
leftQ(Cat' (r1:rs)) w | bypassability(r1) = Union' ([(Cat' [(leftQ r1 w),(Cat' rs)]),(leftQ (Cat' rs) w)]) 
                      | not(bypassability(r1)) = Cat' ([(leftQ (r1) w), (Cat' rs)])
leftQ(Star' r1) w =Cat' ([(leftQ(r1) w),(Star' r1)]) 

-- Implement one more function: proper (cannot recognize epsilon)
proper :: RE' -> Bool
proper Zero = True
proper One = False
proper (Letter' c) = True
proper (Union' rs) = and [ proper r | r <- rs]
proper (Cat' rs) =or [proper r | r <- rs]
proper (Star' r) = False
--proper is written recursively onyl 

-------- Part 2

-- Solve a system of proper linear equations
-- You can assume that the system is correctly formed and proper
solve :: [[RE']] -> [RE'] -> [RE']

solve [] [] = []
solve ((l11:l1J) : rows) (l1':lI') =simp x1 : xI where
  -- l11 is the corner element, and l1J = [l12,...,l1n] is the rest of 1st row
  -- rows are the rest of the rows [[l21,...,l2n], ..., [ln1,...,lnn]]
  -- l1' is the first constant
  -- lI' are the rest of the contants [l2',...,ln']
  
  -- first column [l21, ..., ln1]
  lI1 = map head rows

  -- sub-matrix [[l22,...,l2n], ..., [ln2,...,lnn]]
  lIJ = map tail rows

  -- [[l22_bar,...,l2n_bar], ..., [ln2_bar,...,lnn_bar]] computed via (6)
  lIJ_bar = zipWith3 six lI1 lIJ l1J
  six li1 liJ l1j = map (\lij -> Union' [Cat' [li1, Star' l11, l1j], lij]) liJ

  -- [l2'_bar,..., ln'_bar] computed via (7)
  lI'_bar = zipWith seven lI1 lI'
  seven li1 li' = Union' [Cat' [li1, Star' l11, l1'], li']
    
  -- recursively solve the system of size n-1
  xI = solve lIJ_bar lI'_bar

  -- compute x1 from xI via (5)
  x1 = Cat' [Star' l11, Union' (zipWith (\lij xi -> Cat' [lij, xi]) l1J xI ++ [l1'])]


-- Generate a regular SPLE from an FSM via formulas in Theorem 6.5
toSPLE :: FSM Int -> ([[RE']], [RE'])
toSPLE m = (lIJ, lI') where
  qs = states m
  
  -- Construct matrix of coefficients (coef i j = Lij)
  lIJ = [[simp (coef i j) | j <- qs] | i <- qs] where
  d=delta m
  coef i j |(i,'a',j) `elem` d = Letter' 'a'
           |(i,'b',j)`elem` d = Letter' 'b'
           | otherwise= Zero
  -- Construct constants
  lI' =[ (ans x)|x<-qs]where
    y=finals m
    ans x  |(x `elem` y) =One  
           |otherwise=Zero



-- Convert an FSM to a RE'
conv :: FSM Int -> RE'
conv m = simp $ solution !! start m where
  (matrix, consts) = toSPLE m
  solution = solve matrix consts


-- Test! Test! Test! (and show your tests here)
--emptiness (Union' [Zero,Letter' 'a'])
--emptiness (Union' [Zero])
--emptiness (Zero)
--emptiness(One)
--emptiness (Letter' 'a')
--emptiness (Cat' [Zero])
--emptiness (Cat' [Zero,Letter' 'b'])
--emptiness (Star' Zero)
--unitarity (Union' [Zero,Letter' 'a'])
--unitarity (Union' [Letter' 'a'])
--unitarity (Letter' 'a')
--unitarity (Star' (Letter' 'a'))
--unitarity (Union' [Star' (Zero)])
--unitarity (Union' [Star' (Zero),Star'(Letter' 'a')])
--unitarity (Union' [Star' (Letter' 'b'),Star'(Letter' 'a')])
--unitarity (Union' [Star' (Letter' 'b'),Star'(Zero)])
--unitarity (Union' [Star' (Zero),Star'(Zero)])
--unitarity (Zero)
--unitarity(One)
--unitarity (Letter' 'a')
--unitarity (Cat' [Star' (Zero),Star'(Zero)])
--unitarity (Cat' [Star' (Letter' 'b'),Star'(Zero)])
--unitarity (Cat' [Star' (Letter' 'b'),Star'(Letter' 'a')])
--bypassability (Cat' [Star' (Letter' 'b'),Star'(Letter' 'a')])
--bypassability (Cat' [Star' (Letter' 'b'),Star'(Zero)])
--bypassability(Star' Zero)
--bypassability(Star' (Letter' 'a'))
--bypassability(Zero)
--bypassability(One)
--bypassability(Letter' 'a')
--bypassability (Union' [Star' (Letter' 'b'),Star'(Letter' 'a')])
--bypassability (Union' [Star' (Zero),Star'(Letter' 'a')])
--bypassability (Union' [Star' (Zero),Star'(Zero)])
--bypassability (Union' [(Zero),(Zero)])
--bypassability (Cat' [(Zero),(Zero)])
--bypassability (Cat' [(Letter' 'a'),(Zero)])
--bypassability (Cat' [Star' (Letter' 'a'),(Zero)])
--infiniteness Zero
--infiniteness One
--infiniteness (Letter' 'a')
--infiniteness (Union' [Star' (Zero),Star'(Zero)])
--infiniteness (Union' [Star' (Zero),Star'(Letter' 'a')])
--infiniteness (Union' [Star' (Letter' 'b'),Star'(Letter' 'a')])
--infiniteness (Cat' [Star' (Letter' 'b'),Star'(Letter' 'a')])
--infiniteness (Cat' [Star' (Zero),Star'(Letter' 'a')])
--infiniteness (Cat' [Star' (Zero),Star'(Zero)])
--reversal (One)
--reversal (Zero)
--reversal (Letter' 'b')
--reversal(Union'[Letter' 'a'])
--reversal(Cat'[Letter' 'a'])
--reversal(Star' (Letter' 'a'))
--leftQ One'a'
--leftQ Zero 'b'
--leftQ (Letter' 'b')'b'
--leftQ(Union'[Letter' 'a'])'a'
--leftQ(Cat'[Letter' 'a'])'a'
--proper (Letter' 'a')
--proper (Zero)
--proper (Cat' [Star' (Zero),Star'(Zero)])
--proper (Union' [Star' (Zero),Star'(Zero)])
-- proper (Union' [Star' (Zero),Star'(Letter' 'a')])
--proper (Union' [Star' (Letter' 'b'),Star'(Letter' 'a')])
--proper (Cat' [Star' (Letter' 'b'),Star'(Letter' 'a')])
--proper (Cat' [Star' (Zero),Star'(Letter' 'a')])
--solve[[re"a",re"b",Zero],[re"a",Zero,re"b"],[re"a",Zero,re"b"]][Zero,Zero,One]
--solve[[re"a",re"b",Zero],[re"a",Zero,re"a"],[re"b",Zero,re"b"]][Zero,Zero,One]
--solve[[re"a",re"b",Zero],[re"b",Zero,re"a"],[re"b",Zero,re"a"],[re "b",Zero,re "a"]][One,Zero,One]
--solve[[re"a",re"b",Zero],[re"b",Zero,re"a"]][One,Zero,One]
--solve[[re"a",re"b",Zero],[re"b",Zero,re"a"]][Zero,One]
--solve[[re"a",re"b",Zero],[re"b",Zero,re"a"],[re"b",Zero,re"a"]][Zero,One,One]
--solve[[re"a",re"b",Zero],[re"b",Zero,re"a"],[re"b",Zero,re"a"],[re"b",One,re"b"],[re"a",One,re"b"],[re"a",Zero,re"a"],[re"b",One,re"a"]][Zero,One,One]
--toSPLE FSM{states=[0,1,2], start=0, finals=[2],delta=[(0,'a',0),(0,'b',1),(1,'a',0),(1,'b',2),(2,'a',0),(2,'b',2)]}
--toSPLE FSM{states=[0,1,2,3], start=0, finals=[2,3],delta=[(0,'a',0),(0,'b',1),(1,'a',0),(1,'b',2),(2,'a',0),(2,'b',3),(3,'a',1),(3,'b',3),(1,'a',3)]}
-- toSPLE FSM{states=[0,1,2,3,4,5], start=0, finals=[2,5],delta=[(0,'a',0),(0,'b',1),(1,'a',0),(1,'b',2),(2,'a',0),(2,'b',3),(3,'a',4),(3,'b',3),(1,'a',3),(4,'b',5),(4,'a',4),(5,'a',5),(5,'b',1)]}

--unitarity (Star' Zero)
--EXAMPLE FSM
----FSM{states=[0,1,2], start=0, finals=[2],delta=[(0,'a',0),(0,'b',1),(1,'a',0),(1,'b',2),(2,'a',0),(2,'b',2)]}
---------------- Lab 7 ends here ----------------------------------


{----------------------------------------------------------------------------
| Bonus feature:  A regular-expression simplifier
|----------------------------------------------------------------------------

A "simplified" RE' satisfies the following conditions:
(1) Every Union' is applied to a normalized list of two or more arguments,
    each of which is a One, Letter', Cat', or Star' (i.e., not Zero or Union')
(2) Every Cat' is applied to a list of two or more arguments, each of which is
    a Letter' or Star' (i.e., not Zero, One, Union', or Cat')
(3) Every Star' is applied to a Letter', Union', or Cat' (i.e., not Zero, One,
    or Star')

The following simplification rules, when applied repeatedly at every subterm
of a RE' until the RE' no longer changes, result in a simplified RE':

   Union' []                          --> Zero
   Union' [r]                         --> r
   Union' (rs1 ++ [Zero] ++ rs2)      --> Union' (rs1 ++ rs2)
   Union' (rs1 ++ [Union' rs] ++ rs2) --> Union' (rs1 ++ rs ++ rs2)
   Union' rs                          --> Union' (norm rs)                  (*)

   Cat' []                            --> One
   Cat' [r]                           --> r
   Cat' (rs1 ++ [Zero] ++ rs2)        --> Zero
   Cat' (rs1 ++ [One] ++ rs2)         --> Cat' (rs1 ++ rs2)
   Cat' (rs1 ++ [Union' rs] ++ rs2)   --> Union' (map (\r -> Cat' (rs1 ++ [r] ++ rs2)) rs)
   Cat' (rs1 ++ [Cat' rs] ++ rs2)     --> Cat' (rs1 ++ rs ++ rs2)

   Star' Zero                         --> One
   Star' One                          --> One
   Star' (Star' r)                    --> Star' r

(*) Note: this rule should only be applied if rs is not already normalized;
    normalization is used to realize the commutativity and idempotency of union
    (i.e., that  L1 u L2 = L2 u L1  and  L u L = L ).

However, it would be very inefficient to attempt to apply these rules in the
manner indicated. Instead, our approach is to stage the application of these
rules carefully to minimize the number of traversals of the regular expression.
-------------------------------------------------------------------------------}

simp :: RE' -> RE'
simp Zero = Zero
simp One = One
simp (Letter' c) = Letter' c
simp (Union' rs) = union' $ flat_uni $ map simp rs
simp (Cat' rs) = union' $ flat_cat $ map simp rs
simp (Star' r) = star' $ simp r

-- Smart constructor for Union' that normalizes its arguments and
-- handles the empty and singleton cases
union' :: [RE'] -> RE'
union' rs =  case norm rs of
  []  ->  Zero
  [r] -> r
  rs  -> Union' rs

-- Smart constructor for Cat' that handles the empty and singleton cases
cat' :: [RE'] -> RE'
cat' [] = One
cat' [r] = r
cat' rs = Cat' rs

-- Smart constructor for Star' that handles the constant and Star' cases
star' :: RE' -> RE'
star' Zero = One
star' One = One
star' (Star' r) = star' r
star' r = Star' r

-- Flatten a list of arguments to Union'; assumes each argument is simplified
flat_uni :: [RE'] -> [RE']
flat_uni [] = []
flat_uni (Zero:rs) = flat_uni rs
flat_uni (Union' rs':rs) = rs' ++ flat_uni rs
flat_uni (r:rs) = r : flat_uni rs

-- Flatten a list of arguments to Cat', turning them into a list of arguments
-- to Union'; assumes each argument is simplified
flat_cat :: [RE'] -> [RE']
flat_cat rs = fc [] rs where
  -- fc (args already processed, in reverse order) (args still to be processed)
  fc :: [RE'] -> [RE'] -> [RE']
  fc pr [] = [cat' $ reverse pr]
  fc pr (Zero:rs) = []
  fc pr (One:rs) = fc pr rs
  fc pr (Cat' rs':rs) = fc (reverse rs' ++ pr) rs
  fc pr (Union' rs':rs) = concat $ map (fc pr . (:rs)) rs'
  fc pr (r:rs) = fc (r:pr) rs

