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
package communication.scom;

/**
 *  Command.java - Class for SCOM commands.
 * 
 * @see SCOM Protocol
 * @since 2019.10.28
 * @author deaxuser - Robert Hutter
 */
public final class Command {
    private final String body;
    private String[] args;
    
    /**
     *  Constructor for a Command with no arguments.
     * 
     * @param body Commands body.
     */
    public Command(String body)
    {
        this.body = body;
        this.args = null;
    }
    
    /**
     *  Constructor for command with one argument.
     * 
     * @param body  Commands body.
     * @param arg   Commands argument.
     */
    public Command(String body, String arg)
    {
        this(body, new String[]{arg});
    }
    
    /**
     *  Constructor for a Command with arguments.
     * 
     * @param body Commands body.
     * @param args Arguments.
     */
    public Command(String body, String[] args)
    {
        this.body = body;
        this.args = args;
    }
    
    /**
     *  Add a new argument to the command.
     * 
     * @param arg  The new argument to add.
     */
    public void addArgument(String arg)
    {
        throw new java.lang.UnsupportedOperationException();
    }
    
    /**
     *  Returns the command in its SCOM protocol form.
     * 
     * @return
     */
    @Override
    public String toString()
    {
        StringBuilder sb = new StringBuilder(body);
        sb.append("(");
        for (int i = 0; i < args.length; i++)
        {
            sb.append(args[i]);
            if (i != args.length - 1)
            {
                sb.append(",");
            }
        }
        sb.append(");");
        
        return sb.toString();
    }
    
    public void setArgs(String[] args)
    {
        this.args = args;
    }
    
    public String[] getArgs()
    {
        return this.args;
    }
    
    public String getArg(int argc)
    {
        return this.args[argc];
    }
    
    public String getBody()
    {
        return this.body;
    }
}