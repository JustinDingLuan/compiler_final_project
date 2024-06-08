PROGRAM vartest2;
CONST
  msg1=" local variable (b) from PROCEDURE is ",
  msg2=" local variable (b) from PROGRAM   is ";
VAR
  a;
PROCEDURE proc;
  VAR
    b;
  BEGIN
    b := 123;
    WRITE(msg1,b);
  END;
BEGIN
  CALL proc;
  WRITE(msg2,b);
END.
