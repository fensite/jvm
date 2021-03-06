/**
 * Example 12.4.1-1. 
 * Superclasses Are Initialized Before Subclasses
 */
public class Eg12_4_1_1 {    
    private static class Super {
        static { System.out.print("Super "); }
    }
    private static class One {
        static { System.out.print("One "); }
    }
    private static class Two extends Super {
        static { System.out.print("Two "); }
    }

    public void test() {
        One o = null;
        Two t = new Two();
        System.out.println((Object)o == (Object)t);
//        assertEquals("Super Two false\n", out.toString());
    }
    
    public static void main(String[] args) {
        new Eg12_4_1_1().test();
    }
    
}
