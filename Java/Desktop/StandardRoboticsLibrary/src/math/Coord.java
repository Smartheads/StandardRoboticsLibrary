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
package math;

import java.util.ArrayList;

/**
 *
 * @author deaxuser
 */
public class Coord {
    private int x;
    private int y;
    private int z;
    
    /**
     * Creates a new 2D coordinate.
     * 
     * @param x the coordiante's x component
     * @param y the coordinate's y component
     */
    public Coord (int x, int y)
    {
        this.x = x;
        this.y = y;
        this.z = 1;
    }
    
    /**
     * Creates a new 3D coordinate.
     * 
     * @param x
     * @param y
     * @param z
     */
    public Coord(int x, int y, int z)
    {
        this.x = x;
        this.y = y;
        this.z = z;
    }
    
    /**
     * Parses a text that contains coordinates in the following form:
     * [x],[y]\n
     * 
     * 
     * @param text the text to parse
     * @return
     * Returns and array with the parsed coordinates.
     */
    public static Coord[] parseText (String text)
    {
        ArrayList<Coord> coords = new ArrayList<>();
        ArrayList<String> lines = new ArrayList<>();
        
        text = text.replaceAll("[\\s&&[^\n]]", ""); // Trim whitespaces except for \n
        
        /* Get individual lines */
        StringBuilder sb = new StringBuilder("");
        for (int i = 0; i < text.length(); i++)
        {
            if (text.charAt(i) == '\n')
            {
                lines.add(sb.toString());
                sb = new StringBuilder("");
            }
            else
            {
                sb.append(text.charAt(i));
            }
        }
        
        if (!sb.toString().isEmpty()) // There's no \r after the last coord, but still needs to be added
        {
            lines.add(sb.toString());
        }
        
        /* Parse indiviual lines */
        lines.forEach((String s) -> {
            try
            {
                int cpos = s.indexOf(',');
                coords.add(new Coord(Integer.parseInt(s.substring(0, cpos)),
                        Integer.parseInt(s.substring(cpos + 1, s.length()))));
            }
            catch (NumberFormatException | StringIndexOutOfBoundsException e) {}
        });
        
        return coords.toArray(new Coord[coords.size()]);
    }

    /**
     *
     * @return
     */
    public int getX() {
        return x;
    }

    /**
     *
     * @param x
     */
    public void setX(int x) {
        this.x = x;
    }

    /**
     *
     * @return
     */
    public int getY() {
        return y;
    }

    /**
     *
     * @param y
     */
    public void setY(int y) {
        this.y = y;
    }
    
    /**
     *
     * @return
     */
    public int getZ()
    {
        return this.z;
    }
    
    /**
     *
     * @param z
     */
    public void setZ(int z)
    {
        this.z = z;
    }
}
