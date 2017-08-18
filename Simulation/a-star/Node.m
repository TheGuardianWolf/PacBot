classdef Node < handle
    %NODE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        links = containers.Map;
        position;
        id;
    end
    methods
        function obj = Node(position)
            obj.position = position;
            obj.id = Node.mat2id(position);
        end
        function add_link(self, node, cost)
            if nargin == 1
                cost = 1;
            end
            self.links(node.id) = {node, cost} ;
        end
        function [node, cost] = get_link(self, nodeid)
            [node, cost] = self.links(nodeid);
        end
    end
    methods(Static)
        function r = mat2id(mat)
            r = [int2str(mat(1)) '.' int2str(mat(2))];
        end
        function r = id2mat(id)
            splitid = strsplit(id, '.');
            r = [str2double(splitid(1)), str2double(splitid(2))];
        end
    end
end

