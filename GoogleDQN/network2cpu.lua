require 'cunn'
--require 'nn'
--require 'torch'

local cmd = torch.CmdLine()
cmd:text()
cmd:text('Options:')

cmd:option('-network', '', 'network file')

cmd:text()

local opt = cmd:parse(arg)


local err_msg, exp = pcall(torch.load, opt.network)
if not err_msg then
	print("😓")
	print(err_msg)
	print(exp)
	error("Could not find network file ")
end

best = true

if best and exp.best_model then
    net = exp.best_model
else
    net = exp.model
end

net = cunn.convert(net, nn)
torch.save(opt.network:sub(0,-4).."CPU.t7",net)


