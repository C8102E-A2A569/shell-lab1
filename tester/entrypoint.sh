test_status=0

for test_file in *test*.sh; do
    if [ -f "$test_file" ]; then
        chmod +x "$test_file"
        echo "Running test: $test_file"
        ./"$test_file"
        
        if [ $? -eq 0 ]; then
            echo "Test $test_file completed successfully"
        else
            echo "Test $test_file failed"
            test_status=1
        fi
        
        echo "---------------------------------"
    fi
done

exit $test_status