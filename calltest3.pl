PROGRAM calltest3;
CONST
  msg1 = " please keyin a degree (deg) : ",
  msg2 = " deg = ",
  msg3 = " c = ",
  msg4 = " f = ";
VAR
  deg, f, c;
PROCEDURE cTOf;
  VAR
    fdeg, deg;
  BEGIN
    fdeg := deg*9/5+32;
    f := fdeg;
    deg := fdeg;
    WRITE(deg);
  END;
PROCEDURE fTOc;
  BEGIN
    c := (deg-32)*5/9;
  END;
BEGIN
  WRITE(msg1);
  READ(deg);
  CALL cTOf;
  WRITE(msg2,deg,msg4,f);
  CALL fTOc;
  WRITE(msg2,deg,msg3,c);
  WRITE(fdeg);
END.
