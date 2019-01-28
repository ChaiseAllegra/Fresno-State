-- CSci 119, Lab 4
--Chaise Allegra
---- Regular expressions, along with input and output
data RegExp = Empty
             | Letter Char
             | Union RegExp RegExp
             | Cat RegExp RegExp
             | Star RegExp

instance (Show RegExp) where    -- use precedence to minimize parentheses
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

-- Quick and dirty postfix regex parser, gives non-exaustive match on error
toRE :: String -> RegExp
toRE w = toRE' w [] where
  toRE' [] [r] = r
  toRE' ('+':xs) (r2:r1:rs) = toRE' xs (Union r1 r2:rs)
  toRE' ('.':xs) (r2:r1:rs) = toRE' xs (Cat r1 r2:rs)
  toRE' ('*':xs) (r:rs) = toRE' xs (Star r:rs)
  toRE' ('@':xs) rs = toRE' xs (Empty:rs)
  toRE' (x:xs) rs = toRE' xs (Letter x:rs)


---------------- Part 1 ----------------

-- Implement the six recursive predications/operations on RegExp given in
-- Section 3.3 of the notes. Each should begin with a type declaration.
-- Include several tests for each function.
emptiness :: RegExp -> Bool
emptiness (Empty) = True -- emptiness(Empty) , 
emptiness (Letter a)= False -- emptiness(Letter 'a')
emptiness (Union r1 r2)=emptiness(r1)&&emptiness(r2)--emptiness (Union ((Empty)) ((Empty))) , --emptiness (Union (Letter 'a') (Letter 'b'))
emptiness (Cat r1 r2)=emptiness(r1)||emptiness(r2)--emptiness (Cat ((Empty)) ((Empty))) , emptiness (Cat (Letter 'a') (Letter 'b')) , emptiness (Cat (Letter 'a') ((Empty)))
emptiness(Star r1)=False --emptiness (Star (Letter 'a')) , emptiness(Star(Empty))
--emptiness(toRE "ab+*a.ab+.ab+.")
--emptiness(toRE "@")

unitarity:: RegExp->Bool
unitarity (Empty)= False --unitarity(Empty) , 
unitarity(Letter a)= False --unitarity(Letter 'a')
unitarity(Union r1 r2)=unitarity(r1)&&emptiness(r2)||unitarity(r2)&&emptiness(r1)||unitarity(r1)&&unitarity(r2)--unitarity (Union (Star (Empty)) ((Empty))) , unitarity (Union (Star (Empty)) (Star(Empty))) , unitarity (Union ((Empty)) (Star(Empty)))
unitarity (Cat r1 r2)=unitarity(r1)&&unitarity(r2)-- unitarity (Cat (Letter 'a')(Star(Empty))) , unitarity (Cat (Star(Empty))(Star(Empty)))
unitarity(Star r1)=emptiness(r1)||unitarity(r1) --unitarity (Star(Empty)) , unitarity (Star (Star(Empty)))
--unitarity(toRE "aa.bb.+*") FALSE


bypassability::RegExp->Bool
bypassability (Empty)= False --bypassability(Empty)
bypassability(Letter a)= False --bypassability(Letter 'a')
bypassability(Union r1 r2)=bypassability(r1)||bypassability(r2) -- bypassability(Union (Letter 'a') (Letter 'b')) , bypassability(Union (Star (Letter 'a')) (Letter 'c'))
bypassability(Cat r1 r2)=bypassability(r1)&&bypassability(r2) --bypassability(Cat (Star (Letter 'a')) (Letter 'c')) ,  bypassability(Cat (Star (Letter 'a')) (Star(Letter 'c')))
bypassability(Star r1)=True --bypassability(Star(Letter 'a'))
--bypassability(toRE "b*a.b*.a.*b*.") TRUE
--bypassability(toRE "b*a+b*.") TRUE
--bypassability(toRE "b*a.b*.") FALSE

infiniteness::RegExp->Bool
infiniteness(Empty)= False --infiniteness(Empty)
infiniteness(Letter a)= False --infiniteness(Letter 'a')
infiniteness(Union r1 r2)=infiniteness(r1)||infiniteness(r2) --infiniteness(Union (Letter 'a') (Letter 'c')) , 
infiniteness(Cat r1 r2)=(infiniteness(r1)&&not(emptiness(r2)))||(infiniteness(r2)&&not(emptiness(r1)))
infiniteness(Star r1)=not(emptiness(r1))&&not(unitarity(r1))
--infiniteness(toRE "b*a+b*.") TRUE
--infiniteness(toRE "cdab+++") FALSE



reversal::RegExp->RegExp
reversal(Empty)= Empty --reversal(Empty)
reversal(Letter a)= Letter a --reversal(Letter 'a')
reversal(Union r1 r2)= Union (reversal(r1)) (reversal(r2)) --reversal(Union (Letter 'a') (Letter 'v'))
reversal(Cat r1 r2)= (Cat (reversal(r2)) (reversal(r1))) --reversal(Cat (Letter 'a') (Letter 'v'))
reversal(Star r1)=Star(reversal(r1)) --reversal(Star (Letter 'v'))
--reversal(toRE "cdab+++")

leftQ::RegExp->Char->RegExp
leftQ(Empty) w= Empty --leftQ Empty 'a'
leftQ(Letter a) w | a==w = Star (Empty) --leftQ (Letter 'a') 'a' leftQ (Letter 'a') 'b'
                  | a/=w = Empty
leftQ(Union r1 r2) w=Union (leftQ (r1) w) (leftQ (r2) w) --leftQ(Union(Letter 'a') (Letter 'b')) 'a'
leftQ(Cat r1 r2) w | bypassability(r1) = Union (Cat (leftQ r1 w) r2) (leftQ r2 w) --leftQ(Cat (Letter 'a') (Letter 'b')) 'a' , leftQ(Cat (Letter 'a') (Letter 'b')) 'c' ,  leftQ(Cat (Letter 'a') (Letter 'b')) 'b'
                   | not(bypassability(r1)) = Cat (leftQ r1 w) r2
leftQ(Star r1) w =Cat (leftQ(r1) w) (Star r1) --leftQ(Star (Letter 'a')) 'a' , leftQ(Star (Letter 'a')) 'b'
--leftQ(toRE "cdab...") 'c'  Gives: match@*dab

---------------- Part 2 ----------------

-- Implement the two matching algorithms given in Section 3.4 of the notes.
-- Call them match1 and match2. Start by implementing splits:

-- splits xs = list of all possible splits of xs, in order. For example,
-- splits "abc" = [("","abc"), ("a","bc"), ("ab","c"), ("abc","")]
splits :: [a] -> [([a], [a])]
splits xs = [(x,w) | y <- [0 ..length xs], w <- [drop y xs], x <- [take y xs]]


match1 :: RegExp -> String -> Bool
match1 Empty w = False
match1 (Letter a) w = a `elem` w && ((length w) ==1)
match1 (Union r1 r2) w = (match1 r1 w) || (match1 r2 w)--match1 (Union (Letter 'a') (Letter 'b') )"a" , match1 (Union (Letter 'a') (Letter 'b') )"c"
match1 (Cat r1 r2) w = or [(match1 r1 w1) && (match1 r2 w2) | (w1, w2) <- splits w] --match1 (Cat (Letter 'a') (Letter 'b') )"ab" TRUE
match1 (Star r) w = (w == "")|| or [(match1 r w1) && (match1(Star r) w2) | (w1, w2) <- splits w, w2/=w, w/=""] --match1 (Star (Letter 'a'))"aa" , match1 (Cat (Letter 'a') (Letter 'b') )"a" , match1 (Cat (Letter 'a') (Letter 'b') )"aab"
--match1(toRE "abcd...")"abcd" TRUE
-- match1(toRE "abcd.+.")"abcd" FALSE
--match1(toRE "cd.*f+g.f.")"gf" TRUE
--match1(toRE "cd.*f+g.f.a.b+")"gfa"

strcat :: String -> String -> String
strcat "" ys = ys
strcat xs "" = xs
strcat (x:xs) ys = x:(strcat xs ys)

charToString :: Char -> String
charToString c = [c]

match2 :: RegExp -> String -> Bool
match2 r w = m2 r w null where
  m2 :: RegExp -> String -> ([Char] -> Bool) -> Bool
  m2 Empty w x = True
  m2 (Letter a) w x | w/=(strcat (charToString a) (tail w))=False
                    | w==(strcat (charToString a) (tail w))=True 
  m2 (Union r1 r2) w x = ( m2 r1 w x ||  m2 r2 w x )
  m2 (Cat r1 r2) w x = m2 r1 w (\w' -> m2 r2 w' x)
  m2 r@(Star r1) w x = x w || (m2 r1 w (\w' -> (w' /= w) && m2 r w' x))

--match2(toRE "ab.*c.d.")"cd" TRUE
--match2(toRE "ab.*c.d.")"ab" TRUE
--match2(toRE "ab.*c.d.")"z" FALSE 
--match2(toRE "cd.*d.v.a.n+")"dva"


-- Some regular expressions for testing. Also, test them on other solutions
-- to the exercises of Section 3.2 (as many as you can get).

ab   = toRE "aa.bb.+*"            -- every letter is duplicated
ttla = toRE "ab+*a.ab+.ab+."      -- third to last letter is a
ena  = toRE "b*a.b*.a.*b*."       -- even number of a's
bb1  = toRE "aba.+*b.b.aab.+*."   -- contains bb exactly once