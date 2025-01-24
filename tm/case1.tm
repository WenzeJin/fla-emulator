#Q = {cka,ck0,ckb,ckd,cp,mul1,mul2,reset,illl,ill0,ill1,ill2,ill3,ill4,ill5,ill6,ill7,ill8,ill9,ill10,ill11,ill12,halt}
#S = {a,b}
#G = {a,b,c,_,i,l,e,g,n,p,u,t}
#q0 = ck0
#B = _
#F = {halt}
#N = 3

; check if the input is a*b*
ck0 a__ a__ r** cka
ck0 b__ b__ *** illl
ck0 ___ ___ *** illl
cka a__ a__ r** cka
cka ___ ___ l** illl
cka b__ b__ r** ckb
ckb b__ b__ r** ckb
ckb ___ ___ l** ckd
ckb a__ a__ *** illl
ckd a__ a__ l** ckd
ckd b__ b__ l** ckd
ckd ___ ___ r** cp

; copy a to tape 1, b to tape 2
cp a_* _a* rr* cp
cp b*_ _*b r*r cp
cp _** *** *ll reset

; reset the head of tape 1 and tape 2
reset _a_ _a_ *l* reset
reset __b __b **l reset
reset _ab _ab *ll reset
reset ___ ___ *rr mul1

; mul1: write c to tape 0, if tape 2 is current on b and tape 1 is current on a
mul1 _ab cab rr* mul1
mul1 _a_ _a_ l** halt
mul1 __b __b *lr mul2

; mul2: reset tape 1, do not move tape 2
mul2 _a* _a* *l* mul2
mul2 __* __* *r* mul1


; illegal input
illl a__ ___ l** illl
illl b__ ___ l** illl
illl ___ ___ r** ill0
ill0 *** i** r** ill1
ill1 *** l** r** ill2
ill2 *** l** r** ill3
ill3 *** e** r** ill4
ill4 *** g** r** ill5
ill5 *** a** r** ill6
ill6 *** l** r** ill7
ill7 *** _** r** ill8
ill8 *** i** r** ill9
ill9 *** n** r** ill10
ill10 *** p** r** ill11
ill11 *** u** r** ill12
ill12 *** t** *** halt