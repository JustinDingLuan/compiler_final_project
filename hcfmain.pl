PROGRAM hcfmain;
CONST
  msg1=" keyin a number (a): ",
  msg2=" keyin a number (b): ",
  msg4="L.C.M =";
VAR
  a,b,c,hcf,lcm;
PROCEDURE hcfproc;
  CONST
    msg3="H.C.F =";
  VAR
    big, small;
  PROCEDURE nextdivide;
    VAR
      q, r;
    BEGIN
      q := big/small;
      r := big-small*q;
      big := small;
      small := r;
    END;
  BEGIN
    big := a;
    small := b;
    WHILE small > 0 DO CALL nextdivide;
    WRITE(msg3, big);
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
  CALL lcmproc;
  WRITE(msg4,lcm);
END.
