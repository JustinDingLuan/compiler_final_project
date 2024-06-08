PROGRAM readwrite;
CONST
  msg1=" please key in a number: ",
  msg2=" number keyed in is ",
  msg3=" value doubled is ";
VAR
  n, d;
BEGIN
  WRITE(msg1);
  READ(n);
  WRITE(msg2,n);
  d := n*2;
  WRITE(msg3,d);
END.
