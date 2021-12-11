const SIGSEGV = 0;
const VK_SUCCESS = 0;
const VK_ERROR_INCOMPATIBLE_DRIVER = -9;

const decoder = new TextDecoder('utf-8');
const encoder = new TextEncoder();
let vkObjects = new Array();
let exports = null;
let memory = null;

function setInt32(ptr, value) {
	const buf = new Int32Array(memory, ptr, 1);
	buf.set(value);
}

function getStr(data, length) {
	data = new Uint8Array(memory, data, length);
	return decoder.decode(data);
}

function makeStr(str) {
	str = encoder.encode(str);

	const str_mem = exports.malloc(str.length + 1);
	const buffer = new Uint8Array(memory, str_mem, str.length + 1);

	buffer.set(str);
	buffer.set([0], str.length);

	return str_mem;
}

function makeArgs(module, arg_list) {
	let args = { argc: 1, argv: module + ' ' };

	for (const [key, value] of Object.entries(arg_list)) {
		if (value != false) {
			args.argv += '-' + key + ' ';
			++args.argc;

			if (value != true) {
				args.argv += value + ' ';
				++args.argc;
			}
		}
	}

	return args;
}

const vkAPI = {
	vkCreateInstance: function(create_info, alloc_cb, instance_ptr) {
		const instance = navigator.gpu;

		if(instance === undefined) {
			return VK_ERROR_INCOMPATIBLE_DRIVER;
		}

		setInt32(instance_ptr, vkObjects.length);
		vkObjects.push(instance);

		return VK_SUCCESS;
	},
	vkDestroyInstance: function(instance) {
		vkObjects.splice(instance, 1);
	},
	vkEnumeratePhysicalDevices: async function(instance, dev_count_ptr, dev_ptr) {
		if(dev_ptr != 0) {
			const adapter = await vkObjects[instance].requestAdapter();
			console.log(adapter); //TODO
		} else {
			setInt32(dev_count_ptr, 1);
		}

		return VK_SUCCESS;
	},
	vkGetPhysicalDeviceProperties: function() {

	},
};

const imports = {
	write: function (fd, data, length) {
		switch (fd) {
			case 1: console.log(getStr(data, length)); break;
			case 2: console.error(getStr(data, length)); break;
			default: return -1;
		}

		return length;
	},
	_exit: function (code) {
		throw code;
	},
};

export async function startModule(module, args) {
	const { instance } = await WebAssembly.instantiateStreaming(fetch(module), { env: Object.assign(imports, vkAPI) });

	exports = instance.exports;
	memory = exports.memory.buffer;

	args = makeArgs(module, args);
	args.argv = makeStr(args.argv);

	try {
		exports.libc_init(args.argc, args.argv);
	} catch (code) {
		if (code instanceof WebAssembly.RuntimeError) {
			try {
				exports.call_sig_callback(SIGSEGV);
			} catch (sig_code) {
				code = sig_code;
			}
		}

		if (code != 0) {
			console.warn('Module "' + module + '" returns: ' + code);
		}
	}
}
