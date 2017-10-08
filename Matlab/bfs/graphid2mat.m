function mat = graphid2mat(id)
    split_id = strsplit(id, '.');
    mat = [str2double(split_id(1)), str2double(split_id(2))];
end
