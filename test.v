Require Import Coq.Logic.Classical_Prop.
Require Import Coq.Strings.String.
Require Import Coq.ZArith.ZArith.
Require Import Coq.micromega.Psatz.
Require Import Coq.Classes.Morphisms.
Require Import Coq.Sorting.Permutation.
Require Import Coq.Lists.List. Import ListNotations.
Require Import SetsClass.SetsClass. Import SetsNotation.
Require Import PL.Syntax.
Require Import PL.DenotationsOfExpr.
Require Import PL.DenotationsAsRels.
Require Import PL.Monad.
Require Import PL.Kleene.
Require Import PL.FixedPoint.
Require Import PL.MonadHoare.
Local Open Scope Z.
Local Open Scope list.

Import Monad
       MonadNotation
       SetMonadExamples0
       SetMonadExamples2
       SetMonadExamples3
       SetMonadHoare
       KleeneFix Sets_CPO.
Local Open Scope monad.

Definition monadZadd : Z -> Z -> SetMonad.M Z :=
  fun (x y : Z) => ret(x+y).

Definition monadZmul : Z -> Z -> SetMonad.M Z :=
  fun (x y : Z) => ret(x*y).

(* Infix "+" := add.
Infix "*" := mul. *)

Module test1.

Fixpoint sum(L: list Z) : SetMonad.M Z := 
  match L with
  | nil => ret 0
  | a :: L1 => 
    x <- sum(L1);;
    ret (x+a)
  end.

Definition average(L: list Z): SetMonad.M Z :=
  x <- sum L ;; y <- ret(Z.of_nat (length L)) ;; ret (x/y).

Definition S1 : list Z := 2::2::nil.
Definition S2 : list Z := 4::nil.

Definition cmp: Z -> Z -> SetMonad.M Z :=
  fun x y => ret(1).

Definition main : SetMonad.M unit :=
  a1 <- average S1;;
  a2 <- average S2;;
  _ <- cmp a1 a2;;
  ret tt.

End test1.

Module test2.

Definition n:Z := 1.
Definition x:Z := 1.

Definition body_0 : Z * (Z * Z) -> SetMonad.M (ContinueOrBreak (Z * (Z * Z)) (Z * (Z * Z))) :=
  fun '(b, (a, n)) =>
  choice
    (assume ((n > 0));;
      choice
        (assume (((n mod 2) = 1));;
          b <- ret ((b * a));;
          a <- ret ((a * a));;
          n <- ret ((n / 2));;
          continue (b, (a, n)))
        (assume (~ ((n mod 2) = 1));;
          a <- ret ((a * a));;
          n <- ret ((n / 2));;
          continue (b, (a, n))))
    (assume (~ (n > 0));;
      break (b, (a, n))).

Definition main : SetMonad.M unit :=
  b <- ret (1);;
  a <- ret (x);;
  '(b, (a, n)) <- repeat_break body_0 (b, (a, n));;
  ret tt.



End test2.

Module test3.

Definition print: Z->SetMonad.M Z:= fun x=>ret x.

Definition main : SetMonad.M unit :=
  x <- ret (5);;
  y <- ret (10);;
  z <- ret (((x + y) * 2));;
  choice
    (assume ((z > 25));;
      result <- ret (3);;
      _ <- print result;;
      ret tt)
    (assume (~ (z > 25));;
      result <- ret (2);;
      _ <- print result;;
      ret tt).



End test3.

Module test4.

Definition multiply:Z->Z->Z:=fun x y=>x.
Definition j:Z := 1.
Definition report:Z->SetMonad.M Z:=fun x=>ret x.

Definition body_0 : Z * (Z * Z) -> SetMonad.M (ContinueOrBreak (Z * (Z * Z)) (Z * (Z * Z))) :=
  fun '(sum, (i, j)) =>
  choice
    (assume ((i < 5));;
      j <- ret (0);;
      choice
        (assume ((j < 3));;
          sum <- ret ((sum + multiply i j));;
          j <- ret ((j + 1));;
          continue (sum, (i, j)))
        (assume (~ (j < 3));;
          break (sum, (i, j))))
    (assume (~ (i < 5));;
      break (sum, (i, j))).

Definition main : SetMonad.M unit :=
  sum <- ret (0);;
  i <- ret (0);;
  '(sum, (i, j)) <- repeat_break body_0 (sum, (i, j));;
  _ <- report sum;;
  ret tt.


End test4.


Module test5.

Definition input: Z :=1 .

Definition adjust: Z -> SetMonad.M Z := fun x=> ret x.
Definition output: Z -> SetMonad.M Z := fun x=> ret x.
Definition log: Z->Z->Z -> SetMonad.M Z := fun x y z=> ret x.

Definition main : SetMonad.M unit :=
  a <- ret (input);;
  b <- ret (input);;
  c <- ret (((a * b) mod 10));;
  choice
    (assume (((c = 0) \/ (a > b)));;
      status <- ret (2);;
      _ <- log status a b;;
      _ <- output status;;
      ret tt)
    (assume (~ ((c = 0) \/ (a > b)));;
      status <- ret (3);;
      choice
        (assume (((b mod 2) = 1));;
          correction <- ret ((b + 1));;
          _ <- adjust correction;;
          _ <- output status;;
          ret tt)
        (assume (~ ((b mod 2) = 1));;
          correction <- ret (b);;
          _ <- adjust correction;;
          _ <- output status;;
          ret tt)).

End test5.

Module test6.

Definition data:Z:=1.
Definition current:Z:=1.
Definition counter:Z:=1.
Definition total:Z:=1.
Definition flag:Z:=1.
Definition finalize:Z->Z->SetMonad.M Z:= fun x y => ret x.

Definition body_0 : Z * (Z * (Z * Z)) -> SetMonad.M (ContinueOrBreak (Z * (Z * (Z * Z))) (Z * (Z * (Z * Z)))) :=
  fun '(current, (counter, (total, flag))) =>
  choice
    (assume (((counter < 10) /\ (total < 1000)));;
      current <- ret (data);;
      choice
        (assume ((current = 0));;
          counter <- ret ((counter + 1));;
          continue (current, (counter, (total, flag))))
        (assume (~ (current = 0));;
          total <- ret ((total * current));;
          counter <- ret ((counter + 1));;
          choice
            (assume ((total > 500));;
              flag <- ret (1);;
              continue (current, (counter, (total, flag))))
            (assume (~ (total > 500));;
              flag <- ret (0);;
              continue (current, (counter, (total, flag))))))
    (assume (~ ((counter < 10) /\ (total < 1000)));;
      break (current, (counter, (total, flag)))).

Definition main : SetMonad.M unit :=
  '(current, (counter, (total, flag))) <- repeat_break body_0 (current, (counter, (total, flag)));;
  _ <- finalize total flag;;
  ret tt.

End test6.

Module test7.

(* example7.p is a illigel pseudo code. And main report the problem correctly.*)

End test7.
