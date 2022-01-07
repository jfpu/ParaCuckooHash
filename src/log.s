***** Starting benchmark with 100 operations *****
Benchmarking Unordered Map, 100 Operations...
	Read-Only: 7.0347
	Write-Only: 2.79868

Benchmarking Fine HashMap, 100 Operations...
	Read-Only (24 Reader Threads): 0.239885
TODO: Write Only
	Read-Only Single Bucket (1 Reader Threads): 3.03214
	Read-Only Single Bucket (5 Reader Threads): 1.36576
	Read-Only Single Bucket (9 Reader Threads): 0.565999
	Read-Only Single Bucket (13 Reader Threads): 0.424568
	Read-Only Single Bucket (17 Reader Threads): 0.34498
	Read-Only Single Bucket (21 Reader Threads): 0.280544
TODO: Interleaved Reads / Writes

Benchmarking Cuckoo HashMap, 100 Operations...
	Read-Only:5.86888
	Write-Only: 2.82783
	Read-Only Single Bucket: 11.62

Benchmarking Fine Grained Cuckoo HashMap, 100 Operations...
	Read-Only (24 Reader Threads): 0.243877
	Write-Only: 3.83438
	Read-Only Single Bucket (1 Reader Threads): 2.53262
	Read-Only Single Bucket (5 Reader Threads): 1.32577
	Read-Only Single Bucket (9 Reader Threads): 0.585864
	Read-Only Single Bucket (13 Reader Threads): 0.442771
	Read-Only Single Bucket (17 Reader Threads): 0.353151
	Read-Only Single Bucket (21 Reader Threads): 0.297889

Benchmarking Optimistic Cuckoo HashMap, 100 Operations...
	Read-Only (24 Reader Threads): 0.263193
	Write-Only: 1.06454
	Read-Only Single Bucket (1 Reader Threads): 2.5194
	Read-Only Single Bucket (5 Reader Threads): 1.33638
	Read-Only Single Bucket (9 Reader Threads): 0.623879
	Read-Only Single Bucket (13 Reader Threads): 0.485645
	Read-Only Single Bucket (17 Reader Threads): 0.359684
	Read-Only Single Bucket (21 Reader Threads): 0.295959
TODO: Interleaved Reads / Writes

Benchmarking Optimistic Cuckoo Tag HashMap, 100 Operations...
	Read-Only (24 Reader Threads): 0.252997
	Write-Only: 1.64257
	Read-Only Single Bucket (1 Reader Threads): 2.85096
	Read-Only Single Bucket (5 Reader Threads): 1.35131
	Read-Only Single Bucket (9 Reader Threads): 0.589337
	Read-Only Single Bucket (13 Reader Threads): 0.447146
	Read-Only Single Bucket (17 Reader Threads): 0.362611
	Read-Only Single Bucket (21 Reader Threads): 0.296971

Benchmarking Optimistic Cuckoo Tag Lock LaterHashMap, 100 Operations...
	Read-Only (24 Reader Threads): 0.272155
	Write-Only: 1.39685
	Read-Only Single Bucket (1 Reader Threads): 2.92541
	Read-Only Single Bucket (5 Reader Threads): 1.52105
	Read-Only Single Bucket (9 Reader Threads): 0.634331
	Read-Only Single Bucket (13 Reader Threads): 0.471274
	Read-Only Single Bucket (17 Reader Threads): 0.365308
	Read-Only Single Bucket (21 Reader Threads): 0.300208

Benchmarking DenseHashMap, 100 Operations...
	Read-Only (24 Reader Threads): 0.261076
	Write-Only: 4.39338
	Read-Only Single Bucket (1 Reader Threads): 2.6484
	Read-Only Single Bucket (5 Reader Threads): 1.31071
	Read-Only Single Bucket (9 Reader Threads): 0.555494
	Read-Only Single Bucket (13 Reader Threads): 0.488251
	Read-Only Single Bucket (17 Reader Threads): 0.362505
	Read-Only Single Bucket (21 Reader Threads): 0.302976

Benchmarking Libcuckoo, 100 Operations...
	Read-Only (24 Reader Threads): 0.230766
	Write-Only: 1.10491
	Read-Only Single Bucket: (1 Reader Threads): 1.20702
	Read-Only Single Bucket: (5 Reader Threads): 4.76526
	Read-Only Single Bucket: (9 Reader Threads): 5.02127
	Read-Only Single Bucket: (13 Reader Threads): 5.38795
	Read-Only Single Bucket: (17 Reader Threads): 5.32387
	Read-Only Single Bucket: (21 Reader Threads): 5.78666

Benchmarking Hash Functions, 100 Operations...
	std::hash: 78.2326
	Boost: 18.9707
	hashlittle: 43.1308
	hashlittle2: 39.5777
	MurmurHash2: 85.489
	MurmurHash2A: 53.2874
	MurmurHash3: 38.4854

