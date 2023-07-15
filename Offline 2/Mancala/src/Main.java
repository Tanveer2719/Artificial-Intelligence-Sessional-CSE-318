import java.io.FileWriter;
import java.io.IOException;

public class Main {
    public static void main(String[] args) {
        // FileWriter writer;
        
        // try{ 
        //     writer = new FileWriter("log.txt"); 
        //     writer.write("Heuristic1\tHeuristic2\tWINNER\tLOOSER\n");
        //     for(int i = 1; i<=4; i++){
        //         for(int j = 1; j<=4; j++){
        //             for(int k = 0; k<10; k++){
        //                 if(i == j) break;
        //                 Solve.aiBotVSaiBot(i, j, writer);
        //             }
        //         }
        //         writer.write("***************************\n");
        //         System.out.println(i +" finished");
        //     }
        //     writer.close();
        // } catch (IOException e) {
        //     e.printStackTrace();
        // }

        Solve.playerVSaiBot();

        

    }
}
