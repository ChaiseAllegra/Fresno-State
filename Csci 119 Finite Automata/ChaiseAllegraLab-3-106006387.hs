import Data.List (nub)


---- String operations (note: String = [Char])

-- Length and concatenation (Def 2.2, reimplements len and ++)
strlen :: String -> Int
strlen ""=0
strlen xs = 1 + strlen (tail xs)

strcat :: String -> String -> String
strcat "" ys = ys
strcat xs "" = xs
strcat (x:xs) ys = x:(strcat xs ys)


---- Language operations. Assume inputs contain no duplicates, and insure that
---- outputs also contain no duplicates

type Language = [String]

-- Union of languages
union_lang :: Language -> Language -> Language
union_lang l1 [] = l1
union_lang [] l2 = l2 
union_lang l1 l2 = nub ([ w |w<-l1]++[ x | x<-l2])

-- Concatenation of languages (Def 2.5)
concat_lang :: Language -> Language -> Language
concat_lang l1 [] = l1
concat_lang [] l2=l2
concat_lang l1 l2 = nub([ w++x | x<-l2, w<-l1])


-- L^n = L * L * ... * L (n times)
power_lang :: Language -> Int -> Language -- power_lang l n = undefined
power_lang l 0 = []
power_lang l 1 = l
power_lang l n = concat_lang l (power_lang (l) (n-1))

-- Bounded Kleene star, L* = L^0 U L^1 U L^2 U ... U L^n
bstar_lang :: Language -> Int -> Language
bstar_lang l 0 = [] --[""] e="" L^0 has the empty e complete empty list is []
bstar_lang l 1 = l
bstar_lang l n = bstar_lang(union_lang(l)(power_lang l (n-1)) ) (n-1)


-- Reimplements elem for Languages
element :: String -> Language -> Bool
element [] l = False
element xs [] = False
element xs l =or [ xs==w | w<-l]

-- L1 subset L2
subset :: Language -> Language -> Bool
subset [] l2 = False
subset l2 [] = False
subset l1 l2 = and[ element w l2 | w<-l1]


---- Regular expressions and the languages they denote 
data RegExp = Empty
             | Str String
             | Cat RegExp RegExp
             | Union RegExp RegExp
             | Star RegExp

-- [[r]], except use bound 5 for Kleene star
lang_of :: RegExp -> Language
lang_of (Empty)     = []
lang_of (Str (r))     = r:[ ]
lang_of (Cat (r1) (r2)) = concat_lang (lang_of r1) (lang_of r2)
lang_of (Union (a) (b)) = union_lang (lang_of a) (lang_of b)
lang_of (Star (a))    = bstar_lang (lang_of a ) 5