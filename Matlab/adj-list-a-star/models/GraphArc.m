classdef GraphArc < handle
    %GRAPHNODE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        direction;
        distance;
        food;
        destination;
    end
    
    methods
        function obj = GraphNode(direction, distance, food, destination)
            obj.direction = direction;
            obj.distance = distance;
            obj.food = food;
            obj.destination = destination;
        end
    end
    
end

