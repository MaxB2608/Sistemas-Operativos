//-------------------------------------------------------------------------------------------------------------------------------//
//Bibliotecas utilizadas
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
//-------------------------------------------------------------------------------------------------------------------------------//
public class SopaThreads {
    //---------------------------------------------------------------------------------------------------------------------------//
    //Funcion para buscar palabra en vertical 
    public static boolean buscarVertical(char[][] matriz, String palabra) {
        int n = matriz.length;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= n - palabra.length(); j++) {
                StringBuilder sb = new StringBuilder();
                for (int k = 0; k < palabra.length(); k++) {
                    sb.append(matriz[j + k][i]);
                }
                if (sb.toString().equals(palabra)) {
                    return true;
                }
            }
        }
        return false;
    }
    //---------------------------------------------------------------------------------------------------------------------------//
    // Funcion para buscar la palabra en horizontal
    public static boolean buscarHorizontal(char[][] matriz, String palabra) {
        int n = matriz.length;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= n - palabra.length(); j++) {
                StringBuilder sb = new StringBuilder();
                for (int k = 0; k < palabra.length(); k++) {
                    sb.append(matriz[i][j + k]);
                }
                if (sb.toString().equals(palabra)) {
                    return true;
                }
            }
        }
        return false;
    }
    //---------------------------------------------------------------------------------------------------------------------------//
    //Clase estatica que exitende a las hebras, esto es con el objetivo de hacer uso de las hebras
    static class FuncionRecursiva extends Thread {
        private char [][]matriz;
        private char [][]matriz_copia;
        private int numero;
        String palabra;
        public FuncionRecursiva(char[][] matriz, int numero,String palabra,char [][]matriz_copia) {
            this.matriz = matriz;
            this.numero=numero;
            this.palabra=palabra;
            this.matriz_copia=matriz_copia;
        }
        @Override
        public void run() {
            funcionRecursiva(matriz,numero,palabra,matriz_copia); //Lógica de la función recursiva
        }
    }
    //---------------------------------------------------------------------------------------------------------------------------//
    //Funcion que lo que hace es dividir una matriz inicial la cual la recibe como parametro en 4 submatrices
    public static char[][][] divideMatrix(char[][] originalMatrix) {
        int originalSize = originalMatrix.length;
        int newSize = originalSize / 2;
        char[][][] dividedMatrices = new char[4][newSize][newSize];
        for (int k = 0; k < 4; k++) {
            for (int i = 0; i < newSize; i++) {
                for (int j = 0; j < newSize; j++) {
                    int originalRow = (k < 2) ? i : i + newSize;
                    int originalCol = (k % 2 == 0) ? j : j + newSize;
                    dividedMatrices[k][i][j] = originalMatrix[originalRow][originalCol];
                }
            }
        }
        return dividedMatrices;
    }
    //---------------------------------------------------------------------------------------------------------------------------//
    //Funcion recursiva encargada de crear las hebras recursivamente en el caso que el largo de una matriz sea distinto al de la palabra
    public static void funcionRecursiva(char[][] matriz,int numero,String palabra,char [][]matriz_copia) {
        //------------------------------------------------------------------------------------//
        if(matriz.length==numero){
            if(buscarHorizontal(matriz, palabra)|| buscarVertical(matriz, palabra)){
                t_final=System.currentTimeMillis();
                long tiempo_total=t_final-t_inicio;         //Una vez encuentra la palabra realiza la toma del tiempo para el posterior calculo del tiempo que demoró
                int[]posicion=buscador(matriz_copia,palabra);               //Se busca la palabra en la matriz original
                int posicion1=posicion[0]+1;
                int posicion2=posicion[1]+1;
                System.out.println("Palabra encontrada en la fila "+ posicion1 +" y la columna "+ posicion2);
                System.out.println("Tiempo total de ejecución en ms con Threads: " + tiempo_total);

            }
        //------------------------------------------------------------------------------------//
        }else{
            char[][][] Matrices = divideMatrix(matriz);         //Aqui es donde se genera la recursividad en donde cada hebra recibe una submatriz
            char[][]matriz1=Matrices[0];
            char[][]matriz2=Matrices[1];
            char[][]matriz3=Matrices[2];
            char[][]matriz4=Matrices[3];
            FuncionRecursiva hebra1 = new FuncionRecursiva(matriz1, numero,palabra,matriz_copia);
            FuncionRecursiva hebra2 = new FuncionRecursiva(matriz2, numero,palabra,matriz_copia);
            FuncionRecursiva hebra3 = new FuncionRecursiva(matriz3, numero,palabra,matriz_copia);
            FuncionRecursiva hebra4 = new FuncionRecursiva(matriz4, numero,palabra,matriz_copia);
            hebra1.start();
            hebra2.start();
            hebra3.start();
            hebra4.start();
        }
        //------------------------------------------------------------------------------------//
    }
     //---------------------------------------------------------------------------------------------------------------------------//
    // Las siguientes 2 funciones buscan la palabra en la matriz original, ya que lo que nos importa es
    // el tiempo que tarda en encontrar la palabra, estas funciones se ejecutan luego que la toma de tiempo 
    // de busqueda ya terminó
     //---------------------------------------------------------------------------------------------------------------------------//
    //Busca la palabra en la matriz
    public static int[] buscador(char[][] matrix, String word) {
        //------------------------------------------------------------------------------------//
        int rows = matrix.length;
        int columns = matrix[0].length;
        // Busqueda Horizontal
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j <= columns - word.length(); j++) {
                if (verificacion(matrix, i, j, 0, 1, word)) {
                    return new int[]{i, j};
                }
            }
        }
        // Busqueda Vertical
        for (int i = 0; i < columns; i++) {
            for (int j = 0; j <= rows - word.length(); j++) {
                if (verificacion(matrix, j, i, 1, 0, word)) {
                    return new int[]{j, i};
                }
            }
        }
        return null; 
    }
    //---------------------------------------------------------------------------------------------------------------------------//
    // Verificar si la palabra está presente en la dirección dada
    public static boolean verificacion(char[][] matrix, int x, int y, int dx, int dy, String word) {
        for (int i = 0; i < word.length(); i++) {
            if (matrix[x][y] != word.charAt(i)) {
                return false;
            }
            x += dx;
            y += dy;
        }
        return true;
    }
    //---------------------------------------------------------------------------------------------------------------------------//
    //Se usaran para tomar el tiempo de ejecucion
    private static long t_inicio=0; 
    private static long t_final=0;
    //---------------------------------------------------------------------------------------------------------------------------//
    //Funcion main
    public static void main(String[] args) {
        int largo = 0;
        String palabra = "";
        char[][] matriz = null;
        int largo_pal=0;
        try {
            //------------------------------------------------------------------------------------//
            File file = new File("sopa_de_letras.txt"); // Reemplazar ruta del archivo con la ruta correspondiente segun quien esta ejecutando
            Scanner scanner = new Scanner(file);
            //Se abre el archivo y se comiena a extraer la informacion necesaria como el largo de la matriz, la palabra y la matriz o sopa de letras
            largo = Integer.parseInt(scanner.nextLine());
            palabra = scanner.nextLine();
            largo_pal=palabra.length();
            matriz = new char[largo][largo];
            for (int i = 0; i < largo; i++) {
                String line = scanner.nextLine().replace(" ", ""); 
                for (int j = 0; j < largo; j++) {
                    matriz[i][j] = line.charAt(j);
                }
            }
            scanner.close();
            //------------------------------------------------------------------------------------//
        } catch (FileNotFoundException e) {
            System.out.println("¡Archivo no encontrado!");
        }
        //------------------------------------------------------------------------------------//
        FuncionRecursiva inicio= new FuncionRecursiva(matriz, largo_pal,palabra,matriz);
        t_inicio=System.currentTimeMillis();
        inicio.start();         //Se comienza a ejecutar la hebra principal
        try{
            inicio.join();
        }catch(InterruptedException e){
            e.printStackTrace();
        }
        //------------------------------------------------------------------------------------//
    }
    //---------------------------------------------------------------------------------------------------------------------------//
}