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
 *  Angle.java - Class for mathematical angles.
 * 
 * @since 2018.06.24.
 * @author deaxuser - Robert Hutter
 */
public class Angle {
    private float size;
    private final int vollWinkel;
    
    /**
     *  Constructor of a new angle.
     * 
     * @param size Initial size of the angle
     */
    public Angle (float size)
    {
        this(size, 360);
    }
    
    /**
     *  Constructor of a new angle.
     * 
     * @param size Initial size of the angle
     * @param vollWinkel Max value of the angle. Default: 360
     */
    public Angle (float size, int vollWinkel)
    {
        this.vollWinkel = vollWinkel;
        setAngle (size);
    }
    
    /**
     * Add an angle to the angles size.
     * 
     * @param b Add b to the size of the angle
     */
    public void add (float b)
    {
        setAngle (this.size + b);
    }
    
    /**
     *  Subtract an angle from the angles size,
     * 
     * @param b Subtract b for the sie of the angle
     */
    public void subtract (float b)
    {
        setAngle (this.size - b);
    }
            
    /**
     *  Set the size of the angle.
     * 
     * @param size Set the size of the angle
     */
    public final void setAngle (float size)
    {
        if (size > 0)
        {
            this.size = size % vollWinkel;
        }
        else if (size < 0)
        {
            this.size = vollWinkel - (Math.abs(size) % vollWinkel);
        }
        else
        {
            this.size = 0;
        }
    }
    
    /**
     * 
     * @return
     * 
     * The size of the angle
     */
    public float size ()
    {
        return size;
    }
}
