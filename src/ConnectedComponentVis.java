import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.io.*;
import java.util.*;
import java.security.*;
import javax.swing.*;
import javax.imageio.*;

// --------------------------------------------------------
public class ConnectedComponentVis {
    static int maxS = 500, minS = 50;
    static int maxM = 9, minM = -9;
    static int[] dr = {0, 1, 0, -1};
    static int[] dc = {-1, 0, 1, 0};
    int S;                          // matrix size
    int[][] matrix;                 // input matrix
    int[][] permuted;               // permuted matrix

    // ---------------------------------------------------
    String generate(long seed) {
        try {
            SecureRandom rnd = SecureRandom.getInstance("SHA1PRNG");
            rnd.setSeed(seed);

            S = rnd.nextInt(maxS - minS + 1) + minS;
            if (seed == 1)
                S = 10;
            else if (seed == 2)
                S = minS;
            else if (seed == 3)
                S = maxS;

            SZX = S * SZ + (show_original ? S * SZ + 10 : 0);
            SZY = S * SZ;

            // generate the input matrix
            matrix = new int[S][S];

            // generate a random matrix with some connected components
            int[][] raw = new int[S][S];
            for (int i = 0; i < S; ++i)
                for (int j = 0; j < S; ++j)
                    raw[i][j] = rnd.nextInt(2) == 0 ? 0 : (rnd.nextInt(maxM - minM + 1) + minM);

            int C = rnd.nextInt(S / 10);
            for (int c = 0; c < C; ++c) {
                int K = rnd.nextInt(1 + S * C);
                int i = rnd.nextInt(S);
                int j = rnd.nextInt(S);
                for (int k = 0; k < K; ++k) {
                    raw[i][j] = rnd.nextInt(maxM - minM + 1) + minM;
                    int d = rnd.nextInt(4);
                    int newi = i + dr[d];
                    int newj = j + dc[d];
                    if (newi < 0 || newj < 0 || newi >= S || newj >= S)
                        continue;
                    i = newi;
                    j = newj;
                }
            }

            // shuffle the raw matrix
            int[] perm = new int[S];
            for (int i = 0; i < S; ++i)
                perm[i] = i;
            for (int i = 0; i < S - 1; ++i) {
                int j = rnd.nextInt(S - i) + i;
                int tmp = perm[i];
                perm[i] = perm[j];
                perm[j] = tmp;
            }
            for (int i = 0; i < S; ++i)
                for (int j = 0; j < S; ++j)
                    matrix[i][j] = raw[perm[i]][perm[j]];

            StringBuffer sb = new StringBuffer();
            sb.append(S).append('\n');
            if (seed < 3)
                for (int i = 0; i < S; ++i) {
                    for (int j = 0; j < S; ++j) {
                        if (matrix[i][j] >= 0)
                            sb.append(' ');
                        sb.append(matrix[i][j]).append(' ');
                    }
                    sb.append('\n');
                }
            return sb.toString();
        } catch (Exception e) {
            addFatalError("An exception occurred while generating test case.");
            e.printStackTrace();
            return "";
        }
    }

    // ---------------------------------------------------
    public double runTest(long seed) {
        try {
            String test = generate(seed);
            if (debug)
                System.out.println(test);

            // call user's solution and get return
            int[] mArg = new int[S * S];
            for (int i = 0; i < S; ++i)
                for (int j = 0; j < S; ++j)
                    mArg[i * S + j] = matrix[i][j];

            int[] perm;
            if (proc != null) {
                try {
                    perm = permute(mArg);
                } catch (Exception e) {
                    addFatalError("Failed to get result from permute.");
                    return 0;
                }

                if (perm.length != S) {
                    addFatalError("Your return must contain " + S + " elements, and it contained " + perm.length + ".");
                    return 0;
                }

                // check that this is a valid permutation
                boolean[] used = new boolean[S];
                for (int i = 0; i < S; ++i) {
                    if (perm[i] < 0 || perm[i] >= S) {
                        addFatalError("All elements of your return must be between 0 and " + (S - 1) + ", and your return contained " + perm[i] + ".");
                        return 0;
                    }
                    if (used[perm[i]]) {
                        addFatalError("All elements of your return must be unique, and your return contained " + perm[i] + " twice.");
                        return 0;
                    }
                    used[perm[i]] = true;
                }
            } else {
                // use the identity permutation (for visualization/scoring demo)
                perm = new int[S];
                for (int i = 0; i < S; ++i)
                    perm[i] = i;
            }

            // store permuted matrix separately
            permuted = new int[S][S];
            for (int i = 0; i < S; ++i)
                for (int j = 0; j < S; ++j)
                    permuted[i][j] = matrix[perm[i]][perm[j]];

            // find connected components and calculate score
            int[][] component = new int[S][S];
            int[] r = new int[S * S], c = new int[S * S];
            int nComp = 0, maxSum = -S * S * 10;
            for (int i = 0; i < S; ++i)
                for (int j = 0; j < S; ++j) {
                    if (permuted[i][j] == 0) continue;
                    if (component[i][j] > 0) continue;
                    int size = 0, sum = 0;
                    nComp++;
                    component[i][j] = nComp;
                    r[size] = i;
                    c[size] = j;
                    size++;
                    sum += permuted[i][j];
                    for (int ind = 0; ind < size; ++ind)
                        for (int d = 0; d < 4; ++d) {
                            int newr = r[ind] + dr[d];
                            int newc = c[ind] + dc[d];
                            if (newr < 0 || newc < 0 || newr >= S || newc >= S) continue;
                            if (component[newr][newc] == 0 && permuted[newr][newc] != 0) {
                                component[newr][newc] = nComp;
                                r[size] = newr;
                                c[size] = newc;
                                size++;
                                sum += permuted[newr][newc];
                            }
                        }
                    if (debug)
                        System.out.println("Component " + nComp + " of size " + size + " and sum " + sum);
                    sum *= Math.sqrt(size);
                    if (sum > maxSum) {
                        maxSum = sum;
                        if (vis) {
                            maxComponent = new boolean[S][S];
                            for (int ind = 0; ind < size; ++ind)
                                maxComponent[r[ind]][c[ind]] = true;
                        }
                    }
                }

            if (vis) {
                // draw the image
                jf.setSize(SZX + 10, SZY + 30);
                jf.setVisible(true);
                draw();
            }

            return Math.max(0, maxSum);
        } catch (Exception e) {
            addFatalError("An exception occurred while trying to process your program's results.");
            e.printStackTrace();
            return 0.0;
        }
    }

    // ------------- visualization part ----------------------
    static boolean vis, debug, show_original, save;
    Process proc;
    long seed;
    JFrame jf;
    Vis v;
    InputStream is;
    OutputStream os;
    BufferedReader br;
    // problem-specific drawing params
    int SZX, SZY;
    static int SZ = 10;
    boolean[][] maxComponent;

    // ---------------------------------------------------
    int[] permute(int[] m) throws IOException {
        // pass the params to the solution and get the return
        int i;
        StringBuffer sb = new StringBuffer();
        sb.append(m.length).append('\n');
        for (i = 0; i < m.length; ++i)
            sb.append(m[i]).append('\n');
        os.write(sb.toString().getBytes());
        os.flush();

        // get the return - an array of integers
        int nret = Integer.parseInt(br.readLine());
        int[] ret = new int[nret];
        for (i = 0; i < nret; ++i)
            ret[i] = Integer.parseInt(br.readLine());
        return ret;
    }

    // ---------------------------------------------------
    void draw() {
        if (!vis) return;
        v.repaint();
    }

    // ---------------------------------------------------
    int getColor(int m, boolean highlight) {
        if (m == 0)
            return 0xFFFFFF;
        if (m > 0)
            return (0xF - m * 0x1) * (highlight ? 0x001100 : 0x000011);
        return (0xF + m * 0x1) * (highlight ? 0x110000 : 0x111111);
    }

    // ---------------------------------------------------
    public class Vis extends JPanel implements WindowListener {
        public void paint(Graphics g) {
            try {
                // do painting here
                char[] ch;
                BufferedImage bi = new BufferedImage(SZX + 5, SZY + 5, BufferedImage.TYPE_INT_RGB);
                Graphics2D g2 = (Graphics2D) bi.getGraphics();
                // background
                g2.setColor(new Color(0xD3D3D3));
                g2.fillRect(0, 0, SZX + 10, SZY + 10);

                // draw original matrix
                int off = 0;
                if (show_original) {
                    for (int i = 0; i < S; ++i)
                        for (int j = 0; j < S; ++j) {
                            g2.setColor(new Color(getColor(matrix[i][j], false)));
                            g2.fillRect(j * SZ + off, i * SZ, SZ - 1, SZ - 1);
/*                    g2.setColor(Color.BLACK);
                    ch = ("" + matrix[i][j]).toCharArray();
                    g2.drawChars(ch,0,ch.length, j * SZ + off + 10, i * SZ + 20);*/
                        }
                    off = S * SZ + 10;
                }

                // draw permuted matrix with highlight for the area which forms max component
                for (int i = 0; i < S; ++i)
                    for (int j = 0; j < S; ++j) {
                        g2.setColor(new Color(getColor(permuted[i][j], maxComponent[i][j])));
                        g2.fillRect(j * SZ + off, i * SZ, SZ - 1, SZ - 1);
/*                g2.setColor(Color.BLACK);
                ch = ("" + permuted[i][j]).toCharArray();
                g2.drawChars(ch,0,ch.length, j * SZ + off + 10, i * SZ + 20);*/
                    }

                // draw lines around the area which forms max component
                g2.setColor(Color.BLACK);
                for (int i = 0; i < S; ++i)
                    for (int j = 0; j < S; ++j) {
                        if (!maxComponent[i][j])
                            continue;
                        for (int d = 0; d < 4; ++d) {
                            int r = i + dr[d];
                            int c = j + dc[d];
                            if (r < 0 || c < 0 || r == S || c == S || !maxComponent[r][c]) {
                                if (d == 0)
                                    g2.drawLine(j * SZ + off - 1, i * SZ - 1, j * SZ + off - 1, (i + 1) * SZ - 1);
                                if (d == 1)
                                    g2.drawLine(j * SZ + off - 1, (i + 1) * SZ - 1, (j + 1) * SZ + off - 1, (i + 1) * SZ - 1);
                                if (d == 2)
                                    g2.drawLine((j + 1) * SZ + off - 1, i * SZ - 1, (j + 1) * SZ + off - 1, (i + 1) * SZ - 1);
                                if (d == 3)
                                    g2.drawLine(j * SZ + off - 1, i * SZ - 1, (j + 1) * SZ + off - 1, i * SZ - 1);
                            }
                        }
                    }
                g.drawImage(bi, 0, 0, SZX + 10, SZY + 10, null);
                if (save) {
                    ImageIO.write(bi, "png", new File(seed + ".png"));
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        public Vis() {
            jf.addWindowListener(this);
        }

        // WindowListener
        public void windowClosing(WindowEvent e) {
            if (proc != null)
                try {
                    proc.destroy();
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            System.exit(0);
        }

        public void windowActivated(WindowEvent e) {
        }

        public void windowDeactivated(WindowEvent e) {
        }

        public void windowOpened(WindowEvent e) {
        }

        public void windowClosed(WindowEvent e) {
        }

        public void windowIconified(WindowEvent e) {
        }

        public void windowDeiconified(WindowEvent e) {
        }
    }

    // ---------------------------------------------------
    public double exec(String exec, long seed) {
        //interface for runTest
        if (vis) {
            jf = new JFrame();
            v = new Vis();
            jf.getContentPane().add(v);
        }
        if (exec != null) {
            try {
                Runtime rt = Runtime.getRuntime();
                proc = rt.exec(exec);
                os = proc.getOutputStream();
                is = proc.getInputStream();
                br = new BufferedReader(new InputStreamReader(is));
                new ErrorReader(proc.getErrorStream()).start();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        long start = System.currentTimeMillis();
        double score = runTest(seed);
        long end = System.currentTimeMillis();
        debug("seed", seed, "size", S, "time", end - start, "score", score);
        if (proc != null)
            try {
                proc.destroy();
            } catch (Exception e) {
                e.printStackTrace();
            }
        return score;
    }

    // ---------------------------------------------------
    public static void main(String[] args) {
        if (args.length == 1) {
            for (long seed = 1, end = seed + 10; seed < end; ++seed) {
                double score = new ConnectedComponentVis().exec(args[0], seed);
                if (score < 1) throw new RuntimeException("error : " + seed);
            }
        } else if(args.length == 2) {
            double s0 = 0, s1 = 0;
            for (long seed = 1, end = seed + 10000; seed < end; ++seed) {
                double v0 = new ConnectedComponentVis().exec(args[0], seed);
                double v1 = new ConnectedComponentVis().exec(args[1], seed);
                double max = Math.max(v0, v1);
                s0 += v0 / max;
                s1 += v1 / max;
                debug(s0, s1);
            }
        } else {
            debug("error");
        }
    }

    // ---------------------------------------------------
    void addFatalError(String message) {
        System.out.println(message);
    }

    static void debug(Object... o) {
        System.out.println(Arrays.deepToString(o));
    }
}

class ErrorReader extends Thread {
    InputStream error;

    public ErrorReader(InputStream is) {
        error = is;
    }

    public void run() {
        try {
            byte[] ch = new byte[50000];
            int read;
            while ((read = error.read(ch)) > 0) {
                String s = new String(ch, 0, read);
                System.out.print(s);
                System.out.flush();
            }
        } catch (Exception e) {
        }
    }
}
