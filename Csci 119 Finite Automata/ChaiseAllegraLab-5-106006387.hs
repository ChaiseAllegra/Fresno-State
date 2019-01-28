-- CSci 119, Lab 5
-- Reference: Lecture notes, Sections 4.1, 4.2
import Data.List (nub)
sigma = ['a', 'b']

-- Finite State Machine M = (Q, q0, F, d)
type FSM = ([Int], Int, [Int], [(Int,Char,Int)])

-- Check whether a finite state machine (qs, q0, fs, ts) is correct/complete:
-- (1) States qs are unique (no duplicates)
-- (2) Start state is a state (q0 is in qs)
-- (3) Final states are states (fs is a subset of qs; don't worry about dups)
-- (4) Transition relation is a function from qs and sigma to qs (exactly one
--     output state for each state and letter from sigma)
-- if u change sigma it shoudl work 
--Q:: Set of States it can be in
--	our states can only be number
--q0:: Start state
--F:: Set of final states
--d:: Transition function 
--things needed to make an expression
--1. need QcrossSignma
--2. for every input there is one unique output
-- delta([0,1],0,[1],[(0,'a',1),(0,'b',0),(1,'a',1),(1,'b',1)])

uni :: [Int]->Bool
uni []= True
uni (x:xs) = not(x `elem` xs)&&(uni xs)

--chec kcross product of tis there check of each tuple os unique 
strt::Int->[Int]->Bool
strt w xs= w `elem` xs

f_st ::[Int]->[Int]->Bool
f_st states finals= and [ y `elem` finals| y<-states]

c_prod :: [Int]->[Char]->[(Int,Char)]
c_prod xs ys = [(x,y) | x <- xs, y <- ys]

trn::[(Int,Char,Int)]->[Int]->Bool
trn (si@(x, y, z):hs) hy = and[ or [(x==q)&&(a==y)|(x, y, z)<-(si:hs)] | a<-sigma, q<-hy]&&and[(x `elem` hy)&&(y `elem` sigma)|(x, y, z)<-(si:hs)]


checkFSM :: FSM -> Bool
checkFSM (qs, q0, fs, ts) = (uni qs) && (strt q0 qs) && (f_st fs qs) && (trn ts qs)

-- Gives the transition function of the machine as a function
-- i.e., delta m q a = the state machine m goes to when reading a in state q
--give mahcine state we are in character we about to read and what state do we go to 
delta :: FSM -> Int -> Char -> Int

delta (qs, q0, fs, ts) q a = [z | (x,y,z)<-ts,(x==q),(y==a)]!!0


-- Gives the delta* function
delta_star :: FSM -> Int -> [Char] -> Int
delta_star m q ""=q
delta_star m q (a:w) = delta_star m (delta m q a) w 

-- Machine acceptance, Definition 1 (via delta*)
--pg25
accept1 :: FSM -> [Char] -> Bool
accept1 m@(qs, q0, fs, ts) w = (delta_star m q0 w `elem` fs)


-- Machine acceptance, Definition 2 (via L_q(M))

-- accept2_aux m q w = whether m, starting in q, accepts w (recursive in w)
accept2_aux :: FSM -> Int -> [Char] -> Bool
accept2_aux m@(qs, q0, fs, ts) q "" = (checkFSM m) && (q `elem` fs)
accept2_aux m@(qs, q0, fs, ts) q (a:w) = accept2_aux m (delta m q a) w

-- Defined (non-recursively) in terms of accept2_aux
accept2 :: FSM -> [Char] -> Bool
accept2 m@(qs, q0, fs, ts) w = accept2_aux m q0 w


-- Define a machine that accepts exactly the strings with an even number of a's
-- and test it adequately
even_as :: FSM
even_as=([0,1],0,[0],[(0,'a',1),(0,'b',0),(1,'a',0),(1,'b',1)])

-- Define a machine that accepts exactly the strings that do not contain "aaa"
-- as a substring and test it adequately
trip_a :: FSM
trip_a=([0,1,2,3],0,[0,1,2],[(0,'a',1),(0,'b',0),(1,'a',2),(1,'b',0),(2,'a',3),(2,'b',0),(3,'a',3),(3,'b',3)])