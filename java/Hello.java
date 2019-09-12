import java.io.InputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;

class Hello{
    public static void main(String[] args) throws Exception{
        Hello hello = new Hello();
        InputStream is = hello.getClass().getResourceAsStream("text.txt");
        print(is);
    }
    
    public static void print(InputStream inputStream) throws Exception {
        InputStreamReader reader = new InputStreamReader(inputStream, "utf-8");
        BufferedReader br = new BufferedReader(reader);
        String s = "";
        while ((s = br.readLine()) != null)
            System.out.println(s);
        inputStream.close();
    }
}