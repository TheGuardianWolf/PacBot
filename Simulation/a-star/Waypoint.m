classdef Waypoint < handle
    %WAYPOINT Summary of this class goes here
    %   Detailed explanation goes here
    properties
        node;
        routes = containers.Map;
    end
    
    methods
        function obj = Waypoint(node, routes)
            obj.node = node;
            obj.routes = routes;
        end
        function r = get_position(waypoint)
            r = waypoint.node.position;
        end
        function r = get_id(waypoint)
            r = waypoint.node.id;
        end
        function [nodes, cost] = get_route(waypoint_from, waypoint_to)
            nodes = waypoint_from.routes(waypoint_to);
            cost = length(nodes);
        end
    end
end

