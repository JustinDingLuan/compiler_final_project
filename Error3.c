int GlbVar1 = 20;
char GlbVar2[30] = "ntpucsie";

void cTOf()
{
   int c = 100;
   int f = 0;
   printf("%d" , a);
   if (c > 0)
   {
      f = c * 9 / 5 + 32;
   }
   else
   {
      printf("%d", c);
   }
   printf("%d", f);
}

void fTOc()
{
   int f = 212;
   int c = 0;
   if (f > 0)
   {
      c = (f - 32) * 5 / 9;
   }
   else
   {
      printf("%d", c);
   }
}

void looptest()
{
   int a = 5;
   while (a > 0)
   {
      a = a - 1;
      printf("%d", a);
   }
}

void main()
{
   int a, b = 3, c, d = 5;
   int x = 0;
   int w = abcd;

   printf("%d", GlbVar2);

   do
   {
      b = b - 1;
      printf("%d", b);
   } while (b > 0);

   scanf("%d", &c);

   if (c > 0)
   {
      x = 100;
   }
   else
   {
      x = -100;
   }

   while (d != 0)
   {
      x = x - 1;
      printf("%d", x);
      d = d - 1;
   }

   cTOf();
   looptest();
}
