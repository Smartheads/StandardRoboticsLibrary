/*
 * Copyright (C) 2019 Robert Hutter
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package mapping;

import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author deaxuser Robert Hutter
 */
public class CharMap implements Map2D {
    /**
     * The character array containing the maps data.
     */
    private char[][] map;

    /**
     * A character defining an obstical on the map
     */
    private final char obsticalMark;

    /**
     * A character defining empty space on the map
     */
    private final char emptySpaceMark;
	
    /**
     * Create a new empty map.
     * 
     * @param xDim xDim int width of map
     * @param yDim yDim int height of map
     * @param obM Mark of an obstical on the map
     * @param esM Mark of empty space on the map
     */
    public CharMap (int xDim, int yDim, char obM, char esM)
    {
        this.map = new char[yDim][xDim + 1];

        this.obsticalMark = obM;
        this.emptySpaceMark = esM;

        for (int y = 0; y < yDim; y++)
        {
            for (int x = 0; x < xDim; x++)
            {
                    map[y][x] = emptySpaceMark;
            }
            map[y][xDim] = 10;
        }
    }

    /**
     * Create a map object out of another map object.
     * Used for copying maps.
     * 
     * @param preloaded
     */
    public CharMap (CharMap preloaded)
    {
        this(preloaded.getMap(), preloaded.getObsticalMark(), preloaded.getEmptySpaceMark());
    }
	
    /**
     * Create a map object from a file containing a map
     *
     * @param mapfile
     * @throws IOException
     */
    public CharMap (File mapfile) throws IOException
    {
        this(parseMapfile(mapfile));
    }

    /**
     * Create a new Map out of raw data.
     * 
     * @param raw
     * @param obM
     * @param esM
     */
    public CharMap (char[][] raw, char obM, char esM)
    {
        this.map = raw;
        this.obsticalMark = obM;
        this.emptySpaceMark = esM;
    }
    
    /**
     * A method for simplifying a map with a given length.
     * 
     * @param in
     * @param w
     * @return
     */
    public static CharMap simplfyMap (CharMap in, int w)
    {
        CharMap s = new CharMap((int) in.getWidth() / w, (int) in.getHeight() / w, in.getObsticalMark(), in.getEmptySpaceMark());

        int rY = in.getHeight() % w;
        int rX = in.getWidth() % w;

        for (int y = 0; y < in.getHeight() - rY; y += w)
        {
            for (int x = 0; x < (int) in.getWidth() - rX; x += w)
            {
                char[] b = new char[w*w];
                for (int y2 = 0; y2 < w; y2++)
                {
                    for (int x2 = 0; x2 < w; x2++)
                    {
                        b[x2 + w * y2] = in.getMap()[y2 + y][x2 + x];
                    }
                }

                if (new String (b).contains(Character.toString(in.obsticalMark)))
                {
                     s.setPoint(x / w + 1, y / w + 1, in.obsticalMark);
                }
            }
        }

        return s;
    }
    
    /**
     * Save the contents of a map to a specified destination.
     * 
     * @param dest
     * @throws java.io.IOException
     */
    @Override
    public void exportToFile(File dest) throws IOException
    {        
        FileWriter out = new FileWriter(dest, false);
        /* Write markers */
        out.write (String.valueOf(obsticalMark) + '\n');
        out.write (String.valueOf(emptySpaceMark) + '\n');

        /* Write map raw data */
        for (char[] d1 : map) 
        {
            for (int x = 0; x < d1.length; x++) 
            {
                out.write(d1[x]);
            }
        }

        out.flush();
    }

    /**
     * A method for extracting a map out of a file.
     * 
     * @param mapFile
     * @return
     * @throws IOException
     */
    private static CharMap parseMapfile(File mapFile) throws IOException
    {
        int y;
        List<List<Character>> b;
        char obM;
        char esM;
        
        try (FileReader in = new FileReader(mapFile))
        {
            int c;
            y = 0;
            
            /* Buffer map */
            b = new ArrayList<>();
            b.add(new ArrayList<>());
            obM = (char) in.read();
            in.skip(1);
            esM = (char) in.read();
            in.skip(1);
            while ((c = in.read()) != -1)
            {
                if (c == '\n')
                {
                    y++;
                    b.add(new ArrayList<>());
                }
                else
                {
                    b.get(y).add((char) c);
                }
            }
        }

        int capX = b.get(0).size() + 1;
        int capY = b.size() -1;

        char[][] m = new char[capY][capX];

        for (y = 0; y < capY; y++)
        {
            for (int x = 0; x < capX - 1; x++)
            {
                    m[y][x] = b.get(y).get(x);
            }
            m[y][capX - 1] = 10;
        }

        return new CharMap (m, obM, esM);
    }
	
    /**
     * Sets a rectangular selection of the map to the specified character.
     * 
     * @param x x coordinate of the rectangle's top left corner
     * @param y y coordinate of the rectangle's top left corner
     * @param width the rectangle's width (x axis)
     * @param height the rectangle's height (y axis)
     * @param val the character to set the selection to
     */
    @Override
    public void setPointRectangle (int x, int y, int width, int height, int val)
    {
        for (int y2 = y; y2 < y + height; y2++)
        {
            for (int x2 = x; x2 < x + width; x2++)
            {
                this.setPoint(x2, y2, val);
            }
        }
    }
    
    /**
     * Changes a section of the map according to the specified arguments.
     * 
     * @param x x coordinate of the sections top left corner
     * @param y y coordinate of the sections top left corner
     * @param width the sections width
     * @param val values to replace update map section with
     */
    @Override
    public void setPointRectangle (int x, int y, int width, int[] val)
    {
        int i = 0;
        for (int y2 = y; y2 < y + ((int) val.length / width); y2++)
        {
            for (int x2 = x; x2 < x + width; x2++)
            {
                setPoint(x2, y2, val[i++]);
            }
        }
    }
    
    /**
     * Returns a char[][] containing the requested selection of the map.
     * 
     * @param x x coordinate of the rectangle's top left corner
     * @param y y coordinate of the rectangle's top left corner
     * @param width the rectangle's width (x axis)
     * @param height the rectangle's height (y axis)
     * @return 
     */
    @Override
    public int[][] getPointRectangle(int x, int y, int width, int height)
    {
        int[][] sel = new int[height][width];
        
        int y3 = 0;
        for (int y2 = y; y2 < y + height; y2++)
        {
            int x3 = 0;
            for (int x2 = x; x2 < x + width; x2++)
            {
                sel[y3][x3++] = this.getPoint(x2, y2);
            }
            y3++;
        }
        
        return sel;
    }
    
    /**
     * Sets a square selection of the map to a specific character.
     * 
     * @param x x coordinate of the square's top left corner
     * @param y y coordinate of the square's top left corner
     * @param length the length of one of the square's sides
     * @param val the character to set the selection to
     */
    @Override
    public void setPointSquare (int x, int y, int length, int val)
    {
        this.setPointRectangle(x, y, length, length, val);
    }
	
    /**
     * Set a point of the map to the specified character
     * 
     * @param x the point's x coordinate
     * @param y the point's y coordinate
     * @param val the character to set the point to
     */
    @Override
    public void setPoint (int x, int y, int val)
    {
        map[y-1][x-1] = (char) val;
    }
	
    /**
     * Returns the value of the map's specified point
     * 
     * @param x the point's x coordinate
     * @param y the point's y coordinate
     * @return
     */
    @Override
    public int getPoint (int x, int y)
    {
        return (int) map[y-1][x-1];
    }
	
    /**
     *
     * @return
     */
    public char[][] getMap() {
            return map;
    }
	
    /**
     *
     * @param map
     */
    public void setMap(char[][] map) {
        this.map = map;
    }
	
    /**
     *
     * @return
     */
    @Override
    public int getWidth ()
    {
        return this.map[0].length;
    }
	
    /**
     *
     * @return
     */
    @Override
    public int getHeight ()
    {
        return this.map.length;
    }

    /**
     *
     * @return
     */
    public char getObsticalMark() {
        return obsticalMark;
    }

    /**
     *
     * @return
     */
    public char getEmptySpaceMark() {
        return emptySpaceMark;
    }

    @Override
    public boolean isTraversable(int x, int y) {
        return isTraversable(getPoint(x, y));
    }

    @Override
    public boolean isTraversable(int c) {
        return ((char) c) != obsticalMark;
    }
}
