import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by Alexander on 15.04.2016.
 */
public class Aufgabe4 {
    private static int linearTransform(int x1, int x2, double t) {
        return (int) ((1 - t) * x1 + t * x2);
    }

    private static int quadraticTransform(int x1, int x2, double t) {
        return (int) ((1 - t * t) * x1 + t * t * x2);
    }

    private static List<Point> morphStep(List<Point> polyFrom, List<Point> polyTo, double t,
                                         ITransformFunction transformFunction) {
        assert (polyFrom.size() == polyTo.size());
        List<Point> polygon = new ArrayList<>(polyFrom.size());

        for (int i = 0; i < polyFrom.size(); ++i) {
            int x = transformFunction.transform(polyFrom.get(i).x, polyTo.get(i).x, t);
            int y = transformFunction.transform(polyFrom.get(i).y, polyTo.get(i).y, t);
            polygon.add(new Point(x, y));
        }
        return polygon;
    }

    public static List<List<Point>> morph(List<Point> polyFrom, List<Point> polyTo, double
            stepSize, ITransformFunction transformFunction) {
        List<List<Point>> intermediatePolygons = new ArrayList<>();
        for (double t = 0.0; t < 1.0; t += stepSize) {
            intermediatePolygons.add(morphStep(polyFrom, polyTo, t, transformFunction));
        }
        return intermediatePolygons;
    }

    public static Polygon fromList(List<Point> list) {
        Polygon polygon = new Polygon();
        for (Point p : list) {
            polygon.addPoint(p.x, p.y);
        }
        return polygon;
    }

    public static List<Polygon> fromListOfLists(List<List<Point>> listOfLists) {
        List<Polygon> polygons = new ArrayList<>();
        for (List<Point> list : listOfLists) {
            polygons.add(fromList(list));
        }
        return polygons;
    }

    public static void add(List<Point> list, int x, int y) {
        list.add(new Point(x, y));
    }

    public static void main(String[] args) throws InterruptedException {
        List<Point> polyFrom = new ArrayList<>();
        add(polyFrom, 10, 10);
        add(polyFrom, 10, 100);
        add(polyFrom, 100, 10);
        add(polyFrom, 100, 100);
        List<Point> polyTo = new ArrayList<>();
        add(polyTo, 10, 10);
        add(polyTo, 10, 200);
        add(polyTo, 200, 10);
        add(polyTo, 200, 200);
        double stepSize = 0.005;
        List<Polygon> intermediatePolygons = fromListOfLists(morph(polyFrom, polyTo, stepSize,
                Aufgabe4::linearTransform));

        Frame frame = new Frame();
        frame.setLayout(new BorderLayout());
        Polygon currentPolygon = fromList(polyFrom);
        ProgressBar progressBar = new ProgressBar();
        progressBar.setPreferredSize(new Dimension(-1,40));
        frame.add(progressBar, BorderLayout.NORTH);
        MyCanvas canvas = new MyCanvas();
        frame.add(canvas, BorderLayout.CENTER);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                frame.dispose();
            }
        });
        frame.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ESCAPE) frame.dispose();
            }
        });

        frame.setSize(new Dimension(400, 400));
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);

        Thread t = new Thread(() -> {
            while (frame.isVisible()) {
                for (int i = 0; i < intermediatePolygons.size(); ++i) {
                    Polygon p = intermediatePolygons.get(i);
                    canvas.currentPolygon = p;
                    progressBar.progress = (double) i / intermediatePolygons.size();
                    canvas.repaint();
                    progressBar.repaint();
                    try {
                        Thread.sleep(20);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        });
        t.start();
        t.join();
    }

    public interface ITransformFunction {
        int transform(int x1, int x2, double t);
    }

    private static class MyCanvas extends Canvas {
        Polygon currentPolygon = null;

        @Override
        public void paint(Graphics g) {
            g.clearRect(0, 0, getWidth(), getHeight());
            if (currentPolygon != null)
                g.drawPolygon(currentPolygon);
        }

        //http://stackoverflow.com/questions/10508042/how-do-you-double-buffer-in-java-for-a-game
        @Override
        public void update(Graphics g) {
            Graphics offgc;
            Image offscreen = null;
            Dimension d = getSize();

            // create the offscreen buffer and associated Graphics
            offscreen = createImage(d.width, d.height);
            offgc = offscreen.getGraphics();
            // clear the exposed area
            offgc.setColor(getBackground());
            offgc.fillRect(0, 0, d.width, d.height);
            offgc.setColor(getForeground());
            // do normal redraw
            paint(offgc);
            // transfer offscreen to window
            g.drawImage(offscreen, 0, 0, this);
        }
    }

    private static class ProgressBar extends Canvas {
        double progress;

        @Override
        public void paint(Graphics g) {
            g.clearRect(0, 0, getWidth(), getHeight());
            g.fillRect(0, 0, (int) (getWidth() * progress), getHeight());
        }

        //http://stackoverflow.com/questions/10508042/how-do-you-double-buffer-in-java-for-a-game
        @Override
        public void update(Graphics g) {
            Graphics offgc;
            Image offscreen = null;
            Dimension d = getSize();

            // create the offscreen buffer and associated Graphics
            offscreen = createImage(d.width, d.height);
            offgc = offscreen.getGraphics();
            // clear the exposed area
            offgc.setColor(getBackground());
            offgc.fillRect(0, 0, d.width, d.height);
            offgc.setColor(getForeground());
            // do normal redraw
            paint(offgc);
            // transfer offscreen to window
            g.drawImage(offscreen, 0, 0, this);
        }
    }
}
