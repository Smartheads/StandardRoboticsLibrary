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

/**
 *  Map2D.java - Interface for all 2D maps in SCSRL.
 * 
 * @since 2019.10.25
 * @author deaxuser - Robert Hutter
 */
public interface Map2D {
    void setPoint(int x, int y, int val);
    
    void setPointRectangle(int x, int y, int width, int height, int val);
    void setPointRectangle(int x, int y, int width, int[] val);
    void setPointSquare(int x, int y, int width, int val);
    
    int getPoint(int x, int y);
    int[][] getPointRectangle(int x, int y, int width, int height);
    
    boolean isTraversable(int x, int y);
    boolean isTraversable(int c);
    
    void exportToFile(java.io.File dest) throws java.io.IOException;
    
    int getWidth();
    int getHeight();
}
