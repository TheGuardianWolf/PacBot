classdef WaypointGraph < SimpleGraph
    %WAYPOINTGRAPH Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
    end
    
    methods
        function obj = WaypointGraph(map)
            obj = obj@SimpleGraph({map});
        end
    end
    
end

