public class Board {
    public int[] mancala = new int[14];

    public Board()
    {
        // init board
        for(int i = 0; i < 14; i++){
            if(i == 0 || i == 7){
                mancala[i] = 0;
            }
            else{
                mancala[i] = 4;
            }
        }
    }

    public Board(int[] mancala){
        for(int i = 0; i<14; i++){
            this.mancala[i] = mancala[i];
        }
    }

    public void printBoard(){
        String str = "\t";
        for(int i = 13; i>7; i--){
            str+= mancala[i] + "\t";
        }
        str+= "\n"+mancala[0]+"\t";
        for(int i = 1; i<7; i++){
            str+= "\t";
        }
        str+= mancala[7]+"\n\t";
        for(int i = 1; i<7; i++){
            str+=mancala[i]+"\t";
        }

        System.out.println(str);
    
    }
    
    // i from where the move made
    // returns If there is extra move or not
    public boolean makeMove(int i){

        boolean reMove = false;
         
        // player
        if(i <= 6){
            i = moveBuds(i);
            
            if (i > 0 && i < 7 && mancala[i] == 1 && mancala[14-i] != 0){
                mancala[7] += (1 + mancala[14-i]);
                Helpers.stonesCaptured = mancala[14-i];
                mancala[14-i] = 0;
                mancala[i] = 0;
            }
            if(i == 7){
                reMove = true;
                Helpers.moveEarned ++;
            }
        }
        else{
            // computer
            i = moveBuds(i);
            
            if (i > 7 && i < 14 && mancala[i] == 1 && mancala[14-i] != 0){
                mancala[0] += (1 + mancala[14-i]);
                Helpers.stonesCaptured = mancala[14-i];
                mancala[14-i] = 0;
                mancala[i] = 0;
            }

            if(i == 0){
                reMove = true;
                Helpers.moveEarned ++;

            }
        }

        return reMove;

    }

    private int moveBuds(int i){
        int j = i;
        // transfer the buds to next position untill empty
        while(mancala[j] != 0){
            
            if((i+1) > 13){
                mancala[0] += 1;
                i=0;
            }
            else{
                mancala[++i] += 1;
            }
            mancala[j]--;
        }
        return i;   // return the final position after distribution
    }

    public boolean isGameFinished(){
        if(stonesInMySide() == 0 || stonesInOpponentSide() == 0){
            return true;
        }
        return false;
    }

    public int stonesInMySide(){
        int n = 0;
        for(int i = 8; i<14;i++)
            n += mancala[i];
        return n;     
    }
    
    public int stonesInOpponentSide(){
        int n = 0;
        for(int i = 1; i<7;i++)
            n += mancala[i];
        return n;
    }

}
