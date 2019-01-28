
import Data.List (nub,sort,tails)

sigma = "ab"

data FSM a = FSM {
  states :: [a],
  start  :: a,
  finals :: [a],
  delta  :: [(a,Char,a)]
  }

instance Show a => Show (FSM a) where
  show m = "("   ++ show (states m) ++
           ", "  ++ show (start m)  ++
           ", "  ++ show (finals m) ++
           ", [" ++ steps (delta m) ++ "])" where
    steps [] = []
    steps [t] = step t
    steps (t:ts) = step t ++ "," ++ steps ts
    step (q,c,q') = show q ++ "/" ++ [c] ++ ">" ++ show q'

(><) :: [a] -> [b] -> [(a,b)]
xs >< ys = [(x,y) | x <- xs, y <- ys]   

mynub [] = []
mynub [x] = [x]
mynub (x:ys@(y:zs)) | x == y = mynub ys
                    | otherwise = x : mynub ys



closure :: Ord a => [a] -> (a -> [a]) -> [a]
closure start step = stable $ iterate close (start,[]) where
  stable ((fr,xs):rest) = if null fr then xs else stable rest
  close (fr, xs) = (fr', xs') where  
      xs' = fr ++ xs
      fr' = nub $ filter (`notElem` xs') (concatMap step fr)

minimize :: Ord a => FSM a -> FSM a
minimize (FSM {states=qs, start=s, finals=fs, delta=ts}) = FSM {
  states = qs,
  start  =  s,
  finals = [q | q <- qs, elem q fs],
  delta  = [(q,a,q') | (q,a,q') <- ts]
  } 