import java.io.PrintStream;



class vetor
{
  vetor() {}
  
  public static void main(String[] paramArrayOfString)
  {
    int[] arrayOfInt = new int[10];
    double[] arrayOfDouble = { 2.0D, 3.0D, -5.0D };
    long[] arrayOfLong = { -5L, 3L, 6426246L, -433242L };
    float[] arrayOfFloat = { 2.0F, 3.0F, -5.0F };
    byte[] arrayOfByte = { -2, 4, 0 };
    char[] arrayOfChar = { 'a', '0', ')' };
    short[] arrayOfShort = { 15, 1000, -2 };
    
    for (int i = 0; i < 10; i++) {
      arrayOfInt[i] = i;
    }
    arrayOfInt[0] += 100000;
    for (i = 0; i < 10; i++) {
      System.out.println(arrayOfInt[i]);
    }
    System.out.println(arrayOfInt.length);
    System.out.println();
    

    for (i = 0; i < 3; i++) {
      System.out.println(arrayOfDouble[i]);
    }
    System.out.println(arrayOfDouble.length);
    System.out.println();
    
    for (i = 0; i < 4; i++) {
      System.out.println(arrayOfLong[i]);
    }
    System.out.println(arrayOfLong.length);
    System.out.println();
    
    for (i = 0; i < 3; i++) {
      System.out.println(arrayOfFloat[i]);
    }
    System.out.println(arrayOfFloat.length);
    System.out.println();
    
    for (i = 0; i < 3; i++) {
      System.out.println(arrayOfByte[i]);
    }
    System.out.println(arrayOfByte.length);
    System.out.println();
    
    for (i = 0; i < 3; i++) {
      System.out.println(arrayOfChar[i]);
    }
    System.out.println(arrayOfChar.length);
    System.out.println();
    
    for (i = 0; i < 3; i++) {
      System.out.println(arrayOfShort[i]);
    }
    System.out.println(arrayOfShort.length);
    System.out.println();
  }
