PROGRAM degree;
CONST
  msg = " please keyin a degree (deg) : ",
  msgDeg = " deg = ",
  msgC = " C degree in PROGRAM degree = ",
  msgF = " F degree in PROGRAM degree = ",
  msgF2 = " F degree in PROCEDURE cTOf = ";
VAR
  deg, f, c;
PROCEDURE cTOf;
  VAR
    f;
  BEGIN
    f := deg*9/5+32;
    WRITE(msgF2, f);
  END;
PROCEDURE fTOc;
  VAR
    g;
  BEGIN
    c := (deg-32)*5/9;
  END;
BEGIN
  WRITE(msg);
  READ(deg);
  CALL cTOf;
  CALL fTOc;
  WRITE(msgF, f);
  WRITE(msgC, c);
END.
