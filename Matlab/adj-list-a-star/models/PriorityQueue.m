classdef PriorityQueue < handle
    %PRIORITYQUEUE Summary of this class goes here
    %   Detailed explanation goes here
    % Priority Queue adapted from http://interactivepython.org/runestone/static/pythonds/Trees/BinaryHeapImplementation.html
    properties
        heap;
        size;
    end
    
    methods
        function obj = PriorityQueue(initial_size)
            obj.heap = cell(1, initial_size);
            obj.size = 0;
        end
        
        function perc_up(self, i)
            fd = floor(i / 2);
            while fd > 0
                if self.heap{i}{1} < self.heap{fd}{1}
                    tmp = self.heap{fd};
                    self.heap{fd} = self.heap{i};
                    self.heap{i} = tmp;
                end
                fd = floor(fd / 2);
            end               
        end
        
        function r = min_child(self, i)
            if i * 2 + 1 > self.size
                r = i * 2;
            else
                if self.heap{i * 2}{1} < self.heap{i * 2 + 1}{1}
                    r = i * 2;
                else
                    r = i * 2 + 1;
                end
            end
        end
        
        function perc_down(self, i)
            while (i * 2) <= self.size
                mc = self.min_child(i);
                if self.heap{i}{1} > self.heap{mc}{1}
                    tmp = self.heap{i};
                    self.heap{i} = self.heap{mc};
                    self.heap{mc} = tmp;
                end
                i = mc;
            end
        end
        
        function r = index(self, item)
            for i = 1:self.size
                if self.heap{i}{2} == item
                    r = i;
                    return
                end
            end
            r = 0;
        end
        
        function reprioritise(self, index, priority)
            last_priority = self.heap{index}{1};
            self.heap{index}{1} = priority;
            if priority >  last_priority
                self.perc_down(index);
            elseif priority < last_priority
                self.perc_up(index);
            end
        end
        
        function put(self, priority, item, upsert)
            if upsert == 1
                index = self.index(item);
                if index > 0
                    self.reprioritise(index, priority);
                    return
                end
            end
            self.heap{self.size + 1} = {priority, item};
            self.size = self.size + 1;
            % Realloc needed here in C if exeeds array size
            self.perc_up(self.size);
        end
        
        function [priority, item] = getp(self)
            priority = self.heap{1}{1};
            item = self.heap{1}{2};
            self.heap{1} = self.heap{self.size};
            self.size = self.size - 1;
            self.perc_down(1);
        end
        
        function r = get(self)
            [~, item] = self.getp();
            r = item;
        end
        
        function r = empty(self)
            r = self.size == 0;
        end
    end
    
end

