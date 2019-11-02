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

/**
 *
 * @author deaxuser
 */
public class Vector {
    private double x1;
    private double y1;
    private double z1;
    
    private double x2;
    private double y2;
    private double z2;
    
    private double length;
    
    public Vector (double x1, double y1, double x2, double y2)
    {
        this(x1, y1, 1, x2, y2, 1);
    }
    
    public Vector (double x1, double y1, double z1, double x2, double y2, double z2)
    {
        this.x1 = x1;
        this.y1 = y1;
        this.z1 = z1;
        
        this.x2 = x2;
        this.y2 = y2;
        this.z2 = z2;
        
        updateLength();
    }

    /**
     *
     * @param x
     * @param y
     */
    public Vector (double x, double y)
    {
        this.x1 = x;
        this.y1 = y;
        updateLength();
    }

    /**
     *
     * @param direction
     * @param length
     */
    public Vector (Angle direction, double length)
    {
        this((length * Math.cos(Math.toRadians(90-direction.size()))),
             (length * Math.cos(Math.toRadians(direction.size())))
        );
    }
    
    /**
     *
     * @param a
     * @param b
     * @return
     */
    public static Vector addition(Vector a, Vector b)
    {        
        return new Vector(a.getX1(), a.getY1(), a.getZ1(), b.getX2(), b.getY2(), b.getZ2());
    }
    
    /**
     * 
     * @param vecs
     * @return 
     */
    public static Vector addition(Vector[] vecs)
    {
        Vector v = new Vector (0, 0);
        for (Vector e : vecs)
        {
            v = v.add(e);
        }
        
        return v;
    }
    
    /**
     * 
     * @param b
     * @return 
     */
    public Vector add(Vector b)
    {
        return addition(this, b);
    }
    
    private void updateLength()
    {
        length = Math.sqrt(Math.pow(x1-x2, 2) + Math.pow(y1-y2, 2) + Math.pow(z1-z2, 2));
    }

    /**
     *
     * @return
     */
    public double getLength() {
        return length;
    }

    /**
     *
     * @return
     */
    public double getX1() {
        return x1;
    }

    /**
     *
     * @param x
     */
    public void setX1(double x) {
        this.x1 = x;
        updateLength();
    }

    /**
     *
     * @return
     */
    public double getY1() {
        return y1;
    }

    /**
     *
     * @param y
     */
    public void setY1(double y) {
        this.y1 = y;
        updateLength();
    }

    public double getZ1() {
        return z1;
    }

    public void setZ1(double z1) {
        this.z1 = z1;
        updateLength();
    }

    public double getX2() {
        return x2;
    }

    public void setX2(double x2) {
        this.x2 = x2;
        updateLength();
    }

    public double getY2() {
        return y2;
    }

    public void setY2(double y2) {
        this.y2 = y2;
        updateLength();
    }

    public double getZ2() {
        return z2;
    }

    public void setZ2(double z2) {
        this.z2 = z2;
        updateLength();
    }
}
