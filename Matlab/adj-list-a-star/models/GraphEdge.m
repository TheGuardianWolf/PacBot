classdef GraphEdge < handle
    %GRAPHNODE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        id;
        a1; % Node id and heading pair
        a2;
    end
    
    methods
        function obj = GraphEdge(id, n1, h1, n2, h2)
            obj.id = id;
            obj.a1 = [n1, h1];
            obj.a2 = [n2, h2];
        end
    end
    
end


