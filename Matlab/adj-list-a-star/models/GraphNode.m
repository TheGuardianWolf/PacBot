classdef GraphNode < handle
    %GRAPHNODE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        id;
        food;
        pos_real;
        pos_grid;
    end
    
    methods
        function obj = GraphNode(id, food, grid_x, grid_y, real_x, real_y)
            obj.id = id;
            obj.food = food;
            obj.pos_grid = [grid_x, grid_y];
            obj.pos_real = [real_x, real_y];
        end
    end
    
end

