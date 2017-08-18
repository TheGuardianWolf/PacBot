classdef Waypoint < handle
    %WAYPOINT Summary of this class goes here
    %   Detailed explanation goes here
    properties
        node;
        routes = containers.Map;
    end
    
    methods
        function obj = Waypoint(node)
            obj.node = node;
        end
        function r = get_position(self)
            r = self.node.position;
        end
        function r = get_id(self)
            r = self.node.id;
        end
        function [nodes, cost] = get_route(waypoint_from, waypoint_to)
            nodes = waypoint_from.routes(waypoint_to);
            cost = length(nodes);
        end
    end
end

