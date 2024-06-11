PROGRAM calltest2;
CONST
  msg1=" keyin a number (a): ",
  msg2=" keyin a number (b): ",
  msg3="H.C.F =",
  msg4="L.C.M =";
VAR
  a,b,c,hcf,lcm;
PROCEDURE hcfproc;
  VAR
    big, small, q, r;
  BEGIN
    big := a;
    small := b;
    WHILE small > 0 DO
      BEGIN
        q := big/small;
        r := big-small*q;
        big := small;
        small := r;
      END;
    hcf := big;
  END;
PROCEDURE lcmproc;
  BEGIN
    lcm := a*b/hcf;
  END;
BEGIN
  WRITE(msg1);
  READ(a);
  WRITE(msg2);
  READ(b);
  IF a<b THEN
    BEGIN
      c:=a;
      a:=b;
      b:=c;
    END;
  CALL hcfproc;
  WRITE(msg3,hcf);
  CALL lcmproc;
  WRITE(msg4,lcm);
END.
